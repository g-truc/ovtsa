#include "camera.hpp"
#include "main.hpp"
#include "light_factory.hpp"
#include "object_factory.hpp"
#include "util.hpp"
#include "config.hpp"
#include "adaptator.hpp"

camera::camera() :
	m_fMoveForward(12.f),
	m_fMoveUp(-1.0f),
	m_fRotateX(60.f),
	m_fRotateZ(15.0f)
{}

camera::~camera()
{}

glm::vec3 camera::shade
(
	intersection const & Intersection, 
	material const & Material, 
	glm::vec3 const & View
)
{
    glm::vec3 Color = Material.ambient(Intersection.getGlobalPosition());

    lightFactory & LightFactory = lightFactory::instance();
    for(lightFactory::size_type i = 0; i < LightFactory.size(); i++)
    {
        glm::vec3 LightingColor;
        int iLighting = LightFactory[i]->getRayNumber();
        while(iLighting--)
            LightingColor += LightFactory[i]->shade(Intersection, Material, View);
        Color += LightingColor / float(LightFactory[i]->getRayNumber());
    }
    return Color;   
}

glm::vec3 camera::trace
(
	ray const & RayCopy, 
	int iDepth
)
{
    config & Config = config::instance();
    ray LocalRay;
	ray Ray = RayCopy;
    intersection Intersection;
    intersection NearestIntersection;
    material Material;
    float fDistanceMin = MAX_DISTANCE;

    objectFactory & ObjectFactory = objectFactory::instance();
	for(objectFactory::size_type i = 0; i < ObjectFactory.size(); ++i)
    {
        LocalRay.setPosition(glm::vec3(ObjectFactory[i]->getTransform()->computeInverse(glm::vec4(Ray.getPosition(), 1.0f))));
        LocalRay.setDirection(glm::normalize(glm::vec3(ObjectFactory[i]->getTransform()->computeInverse(glm::vec4(Ray.getDirection(), 0.0f)))));
        if(ObjectFactory[i]->getShape()->intersect(LocalRay, Intersection))
        {
            Intersection.setGlobalPosition(glm::vec3(ObjectFactory[i]->getTransform()->computeMatrix(glm::vec4(Intersection.getLocalPosition(), 1.0f))));
            float fDistance = glm::distance(Ray.getPosition(), Intersection.getGlobalPosition());
            if(fDistance < fDistanceMin)
            {
                fDistanceMin = fDistance;
                NearestIntersection = Intersection;
                glm::vec3 Normal = ObjectFactory[i]->getShape()->computeNormal(Intersection.getLocalPosition(), LocalRay.getDirection());
                NearestIntersection.setNormal(glm::vec3(ObjectFactory[i]->getTransform()->computeNormal(glm::vec4(Normal, 0.0f))));
                Material = *ObjectFactory[i]->getMaterial();
            }
        }
    }

    if(fDistanceMin > MAX_DISTANCE)
		return glm::vec3(1.0f);

    glm::vec3 Color = this->shade(
		NearestIntersection, 
		Material, 
		-glm::vec3(Ray.getDirection())) * Material.getOpacity();

    if(iDepth)
    {
        iDepth--;
        if(Material.getReflectionFactor() > EPSILON && Config.getReflectionRays() > 0)
        {
            Ray.setPosition(NearestIntersection.getGlobalPosition());
            
            glm::vec3 ReflectionColor(0);
            int ReflectionCount = Config.getReflectionRays();
            while(ReflectionCount--)
            {
                Ray.setDirection(glm::reflect(
                    glm::normalize(Ray.getDirection() + glm::vecRand3(0.0f, Config.getReflectionAccuracy())), 
                    glm::normalize(NearestIntersection.getNormal())));
                ReflectionColor += this->trace(Ray, iDepth) * Material.getReflectionFactor();
            }
            Color += ReflectionColor / float(Config.ReflectionRays());
        }

        if(Material.getRefractionFactor() > EPSILON && Config.getRefractionRays() > 0)
        {
            Ray.setPosition(NearestIntersection.getGlobalPosition());
            
            glm::vec3 RefractionColor(0);
            int RefractionCount = Config.getRefractionRays();
            while(RefractionCount--)
            {
                Ray.setDirection(glm::refract(
					glm::normalize(Ray.getDirection() + glm::vecRand3(0.0f, Config.getReflactionAccuracy())), 
						glm::normalize(NearestIntersection.getNormal()), 
						(Ray.getEnvironmentIndex() == 1.0f ? 1.0f / Material.getEnvironmentIndex() : Material.getEnvironmentIndex())));

                Ray.setEnvironmentIndex(Ray.getEnvironmentIndex() == 1.0f ? Material.getEnvironmentIndex() : 1.0f);
                RefractionColor += this->trace(Ray, iDepth) * Material.getRefractionFactor();
            }
            Color += RefractionColor / float(Config.getRefractionRays());
        }
    }
    return Color;
}

void camera::shoot
(
	int iDepth, 
	int iAntialising, 
	glm::uvec2 const & WindowSize
)
{
    config & Config = config::instance();

	glm::mat4 ModelView(1.0f);
    ModelView = glm::translate(0.0f, 0.0f, m_fMoveUp);
    ModelView = glm::rotate(ModelView, m_fRotateZ, 0.0f, 0.0f, 1.0f);
    ModelView = glm::rotate(ModelView, m_fRotateX, 1.0f, 0.0f, 0.0f);
    ModelView = glm::translate(ModelView, 0.0f, 0.0f, m_fMoveForward);

    m_WindowSize = WindowSize;
    //if(pConfig->AntiAliasingType() == AA_ADAPT && iAntialising > 1)
    //    ShootAntiAliasingAdaptative(iDepth, iAntialising);
    //else if(pConfig->AntiAliasingType() == AA_FORCE && iAntialising > 1)
    //    ShootAntiAliasing(iDepth, iAntialising);
    //else
    //    ShootAliasing(iDepth);

    if(iAntialising > 1)
        //this->shootAntiAliasingAdaptative(ModelView, iDepth, iAntialising);
		this->shootAntiAliasing(ModelView, iDepth, iAntialising);
    else
        this->shootAliasing(ModelView, iDepth);
}

void camera::shootAliasing
(
	glm::mat4 const & ModelView,
	int iDepth
)
{
    config & Config = config::instance();

	surface Surface(m_WindowSize);

    ray Ray;
    Ray.setEnvironmentIndex(1.0f);

	std::size_t Total = glm::compMul(m_WindowSize);
	std::size_t Count = 0;

	//#pragma omp parallel for
    for(int y = -int(m_WindowSize.y) / 2; y < int(m_WindowSize.y) / 2; y++)
	{
		printf("%2.3f%\r", float(Count) / float(Total) * 100.f);

		for(int x = -int(m_WindowSize.x) / 2; x < int(m_WindowSize.x) / 2; x++)
	    {
	        Ray.setDirection(glm::vec3(ModelView * glm::normalize(glm::vec4(float(x), float(y), -float(m_WindowSize.y), 0.0f))));
	        Ray.setPosition(glm::vec3(ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			glm::vec3 Color = this->trace(Ray, iDepth);
			Surface.add(glm::uvec2(x, y) + glm::uvec2(m_WindowSize / glm::uint(2)), Color);
			Count++;
	    }
	}

	Surface.SaveAs(Config.File());
}

void camera::shootAntiAliasing
(
	glm::mat4 const & ModelView,
	int iDepth, 
	int Antialising
)
{
    ray Ray;
    Ray.setEnvironmentIndex(1.0f);

    surface Surface(m_WindowSize);

	std::vector<glm::vec2> AntialisingBias(Antialising);
    for(std::vector<glm::vec2>::size_type i = 0; i < AntialisingBias.size(); i++)
        AntialisingBias[i] = glm::compRand2(-0.5f, 0.5f);
	//AntialisingBias[0] = glm::vec2(-1.0f);
	//AntialisingBias[1] = glm::vec2(1.0f);

	std::size_t Total = glm::compMul(m_WindowSize) * AntialisingBias.size();
	std::size_t Count = 0;
    for(std::vector<glm::vec2>::size_type i = 0; i < AntialisingBias.size(); i++)
    {
		//#pragma omp parallel for
        for(int y = -int(m_WindowSize.y) / 2; y < int(m_WindowSize.y) / 2; y++)
		{
			printf("%2.3f%\r", float(Count) / float(Total) * 100.f);

			for(int x = -int(m_WindowSize.x) / 2; x < int(m_WindowSize.x) / 2; x++)
	        {
				glm::ivec2 WindowPosition(x, y);
	            Ray.setDirection(glm::vec3(ModelView * glm::normalize(glm::vec4(glm::vec2(WindowPosition) + AntialisingBias[i], -float(m_WindowSize.y), 0.0f))));
	            Ray.setPosition(glm::vec3(ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	            //Ray.setDirection(glm::vec3(ModelView * glm::normalize(glm::vec4(glm::vec2(WindowPosition), -float(m_WindowSize.y), 0.0f))));
	            //Ray.setPosition(glm::vec3(ModelView * glm::vec4(AntialisingBias[i], 0.0f, 1.0f)));
				glm::vec3 Color = this->trace(Ray, iDepth) / float(Antialising);
				Surface.add(glm::uvec2(x, y) + glm::uvec2(m_WindowSize / glm::uint(2)), Color);
				Count++;
	        }
		}
    }

    Surface.SaveAs(config::instance().File());
}

/*
void camera::shootAliasing(int iDepth)
{
	config* pConfig = config::Instance();
	Image.New(pConfig->WindowWidth(), pConfig->WindowHeight(), 24);

	CRay Ray;
	Ray.EnvironmentIndex() = 1.0f;
	int nbThread = 4;//getenv("RAYTRACER_NB_THREAD") ? atoi(getenv("RAYTRACER_NB_THREAD")) : 1;
	std::vector<boost::thread *> threads;
	threads.resize(nbThread);
	int yStep = m_WindowSize.y / nbThread;
	for(int i = 0; i < nbThread; i++)
	{
		int yStart = -1 *  m_WindowSize.y / 2 + i * yStep;
		threads[i] = new boost::thread(boost::bind<void>(boost::mem_fn(&Camera::ShootAntiAliasingInThread), this, -1 * m_WindowSize.x / 2, m_WindowSize.x / 2,
		yStart, yStart + yStep, iDepth));
	}
	for (int i = 0; i < nbThread; i++)
		threads[i]->join ();
	Image.SaveAs(pConfig->File().c_str());
}
*/
/*
void camera::shootAntiAliasingInThread (int bx, int ex, int by, int ey, int iDepth)
{
	CRay Ray;
	Ray.EnvironmentIndex() = 1.0f;
	int iAntialising = 1;
	for(int y = by; y < ey; y++)
	for(int x = bx; x < ex; x++)
	{
		Ray.Direction() = glm::vec3(m_ModelView * glm::normalize(glm::vec4(float(x), float(y), -float(m_WindowSize.y), 0.0f)));
		Ray.Origin() = glm::vec3(m_ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 Color = Trace(Ray, iDepth);
		//boost::mutex::scoped_lock lock (shootMutex_);
		Image.SetValue(x+ m_WindowSize.x / 2, y + m_WindowSize.y / 2, Color);
	}
}
*/
/*
void camera::shootAntiAliasing(int iDepth, int iAntialising)
{
    CRay Ray;
    Ray.EnvironmentIndex() = 1.0f;

    CSurface Surface;
    Surface.Init(m_WindowSize.x, m_WindowSize.y);

    int nThread = 1;
    std::vector<boost::thread *> threads;
    for(int i = 0; i < iAntialising; i++)
    {
      depth_ = iDepth;
      std::cout << "Launching thread " << i << std::endl;
      threads[i] = new boost::thread(boost::bind<void>(boost::mem_fn(&Camera::ShootInThread),
						       this,
						       i));
    }

    threads[0] = new boost::thread(boost::bind<void>(boost::mem_fn(&Camera::AdaptInThread),
						      this,
						      0, m_WindowSize.x / 2,
						      0, m_WindowSize.y / 2));
    threads[1] = new boost::thread(boost::bind<void>(boost::mem_fn(&Camera::AdaptInThread),
						      this,
						      m_WindowSize.x / 2, m_WindowSize.x,
						      m_WindowSize.y / 2, m_WindowSize.y));
    threads[2] = new boost::thread(boost::bind<void>(boost::mem_fn(&Camera::AdaptInThread),
						      this,
						      0, m_WindowSize.x / 2,
						      m_WindowSize.y / 2, m_WindowSize.y));
    threads[3] = new boost::thread(boost::bind<void>(boost::mem_fn(&Camera::AdaptInThread),
						      this,
						      m_WindowSize.x / 2, m_WindowSize.x,
						      0, m_WindowSize.y / 2));

    Surface.SaveAs(config::Instance()->File().c_str());
}

void camera::adaptInThread (int bx, int ex,
			    int by, int ey)
{
  int iAntialising = 1;
  std::cout << "Adapt in rect " << bx << ";" << ex << ";" << by << ";" << ey << std::endl;
  for (int y = by;
       y < ey;
       y++)
    for (int x = bx;
	 x < ex;
	 x++)
      {
	boost::mutex::scoped_lock lock (shootMutex_);
	Surface.Adapt(x, y, iAntialising);
      }
}
*/
/*
void camera::shootInThread (int i)
{
    CRay Ray;
    Ray.EnvironmentIndex() = 1.0f;
    glm::vec2 pAnti;
    pAnti.x = glm::compRand1(0.0f, 1.0f) * 2.0f - 1.0f;
    pAnti.y = glm::compRand1(0.0f, 1.0f) * 2.0f - 1.0f;

	for(int y = -m_WindowSize.y / 2; y < m_WindowSize.y / 2; y++)
    for(int x = -m_WindowSize.x / 2; x < m_WindowSize.x / 2; x++)
    {
        Ray.Direction() = glm::vec3(m_ModelView * glm::normalize(glm::vec4(float(x + pAnti.x), float(y + pAnti.y), -float(m_WindowSize.y), 0.0f)));
        Ray.Origin() = glm::vec3(m_ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		boost::mutex::scoped_lock lock (shootMutex_);
        Surface.Add(x + m_WindowSize.x / 2, y + m_WindowSize.y / 2, Trace(Ray, depth_));
    }
}
*/
bool camera::checkAliasing
(
	surface const & Surface, 
	adaptator const & Adaptator, 
	int x, int y
)
{
    config & Config = config::instance();

    if(x < 1 || x >= int(m_WindowSize.x) || y < 1 || y >= int(m_WindowSize.y))
        return false;

	glm::vec3 ColorMM = Surface.getTexel(glm::uvec2(x - 1, y - 1)) / Adaptator.getFactor(glm::uvec2(x - 1, y - 1));
    glm::vec3 ColorM0 = Surface.getTexel(glm::uvec2(x - 1, y + 0)) / Adaptator.getFactor(glm::uvec2(x - 1, y + 0));
    glm::vec3 ColorMP = Surface.getTexel(glm::uvec2(x - 1, y + 1)) / Adaptator.getFactor(glm::uvec2(x - 1, y + 1));

    glm::vec3 Color0M = Surface.getTexel(glm::uvec2(x + 0, y - 1)) / Adaptator.getFactor(glm::uvec2(x + 0, y - 1));
    glm::vec3 Color00 = Surface.getTexel(glm::uvec2(x + 0, y + 0)) / Adaptator.getFactor(glm::uvec2(x + 0, y + 0));
    glm::vec3 Color0P = Surface.getTexel(glm::uvec2(x + 0, y + 1)) / Adaptator.getFactor(glm::uvec2(x + 0, y + 1));

    glm::vec3 ColorPM = Surface.getTexel(glm::uvec2(x + 1, y - 1)) / Adaptator.getFactor(glm::uvec2(x + 1, y - 1));
    glm::vec3 ColorP0 = Surface.getTexel(glm::uvec2(x + 1, y + 0)) / Adaptator.getFactor(glm::uvec2(x + 1, y + 0));
    glm::vec3 ColorPP = Surface.getTexel(glm::uvec2(x + 1, y + 1)) / Adaptator.getFactor(glm::uvec2(x + 1, y + 1));

    if(glm::any(glm::greaterThan(glm::abs(ColorMM - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    if(glm::any(glm::greaterThan(glm::abs(ColorM0 - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    if(glm::any(glm::greaterThan(glm::abs(ColorMP - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    if(glm::any(glm::greaterThan(glm::abs(Color0M - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    if(glm::any(glm::greaterThan(glm::abs(Color0P - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    if(glm::any(glm::greaterThan(glm::abs(ColorPM - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    if(glm::any(glm::greaterThan(glm::abs(ColorP0 - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    if(glm::any(glm::greaterThan(glm::abs(ColorPP - Color00), glm::vec3(Config.AntiAliasingAccuracy()))))
        return true;
    return false;
}

void camera::shootAntiAliasingAdaptative
(
	glm::mat4 const & ModelView,
	int iDepth, 
	int iAntialising
)
{
    adaptator Adaptator(m_WindowSize);

    ray Ray;
    Ray.setEnvironmentIndex(1.0f);

    surface Surface(m_WindowSize);

    glm::vec2 *pAnti = new glm::vec2[iAntialising];
    for(int i = 0; i < iAntialising; i++)
    {
        pAnti[i].x = glm::compRand1(0.0f, 1.0f) * 2.0f - 1.0f;
        pAnti[i].y = glm::compRand1(0.0f, 1.0f) * 2.0f - 1.0f;
    }

	std::size_t Total = glm::compMul(m_WindowSize) * iAntialising;
	std::size_t Count = 0;
    for(int i = 0; i < iAntialising; i++)
    {
		for(int y = -int(m_WindowSize.y / 2); y < int(m_WindowSize.y / 2); y++)
		{
			printf("%2.3f%\r", float(Count) / float(Total) * 100.f);

			for(int x = -int(m_WindowSize.x / 2); x < int(m_WindowSize.x / 2); x++)
	        {
	            int x_tmp = x + m_WindowSize.x / 2;
	            int y_tmp = y + m_WindowSize.y / 2;
	            bool bAnti = true;
	            if(i > 0)
	                bAnti = this->checkAliasing(Surface, Adaptator, x_tmp, y_tmp);
	            if(bAnti)
	            {
	                // 05/02/2005 - A same value is used to depth and height.
	                Ray.setDirection(glm::vec3(ModelView * glm::normalize(glm::vec4(float(x + pAnti[i].x), float(y + pAnti[i].y), -float(m_WindowSize.y), 0.0f))));
	                Ray.setPosition(glm::vec3(ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	                Surface.add(glm::uvec2(x_tmp, y_tmp), this->trace(Ray, iDepth));
	                Adaptator.add(glm::uvec2(x_tmp, y_tmp));
					Count++;
	            }
	        }
		}
    }

	for(glm::uint y = 0; y < m_WindowSize.y; y++)
    for(glm::uint x = 0; x < m_WindowSize.x; x++)
        Surface.div(glm::uvec2(x, y), Adaptator.getFactor(glm::uvec2(x, y)));

    delete[] pAnti;

    Surface.SaveAs(config::instance().File());
}

void camera::rotateX
(
	action const & Move
)
{
    switch(Move)
    {
    case PLUS:
        m_fRotateX += 5;
        break;
    case MINUS:
        m_fRotateX -= 5;
        break;
    }
}

void camera::rotateZ
(
	action const & Move
)
{
    switch(Move)
    {
    case PLUS:
        m_fRotateZ += 5;
        break;
    case MINUS:
        m_fRotateZ -= 5;
        break;
    }
}

void camera::move
(
	action const & Move
)
{
    switch(Move)
    {
    case PLUS:
        m_fMoveForward++;
        if(m_fMoveForward > 24) 
            m_fMoveForward = 24; 
        break;
    case MINUS:
        m_fMoveForward--;
        if(m_fMoveForward < 1) 
            m_fMoveForward = 1; 
        break;
    }
}



