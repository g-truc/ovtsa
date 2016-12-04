#include "camera.hpp"
#include "main.hpp"
#include "light_factory.hpp"
#include "object_factory.hpp"
#include "util.hpp"
#include "config.hpp"
#include "adaptator.hpp"

camera::camera() :
	MoveForward(12.f),
	MoveUp(-1.0f),
	Rotate(glm::radians(60.f), 0.0f, glm::radians(15.f))
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

	light_factory& LightFactory = light_factory::instance();
	for(light_factory::size_type i = 0; i < LightFactory.size(); i++)
	{
		glm::vec3 LightingColor;
		int Lighting = LightFactory[i]->getRayNumber();
		while(Lighting--)
			LightingColor += LightFactory[i]->shade(Intersection, Material, View);
		Color += LightingColor / float(LightFactory[i]->getRayNumber());
	}
	return Color;
}

glm::vec3 camera::trace(ray const& RayCopy, int iDepth)
{
	config& Config = config::instance();
	ray LocalRay;
	ray Ray = RayCopy;
	intersection Intersection;
	intersection NearestIntersection;
	material Material;
	float fDistanceMin = MAX_DISTANCE;

	object_factory& ObjectFactory = object_factory::instance();
	for(object_factory::size_type i = 0, n = ObjectFactory.size(); i < n; ++i)
	{
		LocalRay.set_position(glm::vec3(ObjectFactory[i]->getTransform()->computeInverse(glm::vec4(Ray.get_position(), 1.0f))));
		LocalRay.set_direction(glm::normalize(glm::vec3(ObjectFactory[i]->getTransform()->computeInverse(glm::vec4(Ray.get_direction(), 0.0f)))));
		if(ObjectFactory[i]->getShape()->intersect(LocalRay, Intersection))
		{
			Intersection.setGlobalPosition(glm::vec3(ObjectFactory[i]->getTransform()->computeMatrix(glm::vec4(Intersection.getLocalPosition(), 1.0f))));
			float fDistance = glm::distance(Ray.get_position(), Intersection.getGlobalPosition());
			if(fDistance < fDistanceMin)
			{
				fDistanceMin = fDistance;
				NearestIntersection = Intersection;
				glm::vec3 Normal = ObjectFactory[i]->getShape()->compute_normal(Intersection.getLocalPosition(), LocalRay.get_direction());
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
		-glm::vec3(Ray.get_direction())) * Material.getOpacity();

	if(iDepth)
	{
		iDepth--;
		if(Material.getReflectionFactor() > glm::epsilon<float>() * 100.f && Config.getReflectionRays() > 0)
		{
			Ray.set_position(NearestIntersection.getGlobalPosition());

			glm::vec3 ReflectionColor(0);
			int ReflectionCount = Config.getReflectionRays();
			while(ReflectionCount--)
			{
				Ray.set_direction(glm::reflect(
					glm::normalize(Ray.get_direction()/* + glm::sphericalRand(Config.getReflectionAccuracy())*/), 
					glm::normalize(NearestIntersection.getNormal())));
				ReflectionColor += this->trace(Ray, iDepth) * Material.getReflectionFactor();
			}
			Color += ReflectionColor / float(Config.getReflectionRays());
		}

		if(Material.getRefractionFactor() > glm::epsilon<float>() * 100.f && Config.getRefractionRays() > 0)
		{
			Ray.set_position(NearestIntersection.getGlobalPosition());

			glm::vec3 RefractionColor(0);
			int RefractionCount = Config.getRefractionRays();
			while(RefractionCount--)
			{
				Ray.set_direction(glm::refract(
					glm::normalize(Ray.get_direction()/* + glm::sphericalRand(Config.getReflactionAccuracy())*/), 
						glm::normalize(NearestIntersection.getNormal()), 
						(Ray.get_environment_index() == 1.0f ? 1.0f / Material.getEnvironmentIndex() : Material.getEnvironmentIndex())));

				Ray.set_environment_index(Ray.get_environment_index() == 1.0f ? Material.getEnvironmentIndex() : 1.0f);
				RefractionColor += this->trace(Ray, iDepth) * Material.getRefractionFactor();
			}
			Color += RefractionColor / float(Config.getRefractionRays());
		}
	}
	return Color;
}

void camera::shoot(int iDepth, int iAntialising, glm::uvec2 const & WindowSize)
{
	config & Config = config::instance();

	glm::mat4 ModelView(1.0f);
	ModelView = glm::translate(ModelView, glm::vec3(0.0f, 0.0f, MoveUp));
	ModelView = glm::rotate(ModelView, this->Rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelView = glm::rotate(ModelView, this->Rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelView = glm::translate(ModelView, glm::vec3(0.0f, 0.0f, MoveForward));

	this->WindowSize = WindowSize;
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

gli::texture2d scale_clamp(gli::texture2d const& Surface, float MinVal, float MaxVal)
{
	gli::texture2d SurfaceClamped(Surface.format(), Surface.extent(), 1);

	gli::sampler2d<float> SamplerWrite(SurfaceClamped, gli::WRAP_CLAMP_TO_EDGE);
	gli::sampler2d<float> SamplerFetch(Surface, gli::WRAP_CLAMP_TO_EDGE);

	for(std::size_t y = 0; y < SurfaceClamped.extent().y; ++y)
	for(std::size_t x = 0; x < SurfaceClamped.extent().x; ++x)
	{
		glm::vec4 const& Color = SamplerFetch.texel_fetch(gli::extent2d(x, y), 0);
		SamplerWrite.texel_write(gli::extent2d(x, y), 0, glm::clamp(Color, MinVal, MaxVal));
	}

	return SurfaceClamped;
}

void camera::shootAliasing(glm::mat4 const& ModelView, int Depth)
{
	config& Config = config::instance();

	gli::texture2d Surface(gli::FORMAT_RGBA32_SFLOAT_PACK32, this->WindowSize, 1);

	ray Ray;
	Ray.set_environment_index(1.0f);

	std::size_t Total = glm::compMul(this->WindowSize);
	std::size_t Count = 0;

	//#pragma omp parallel for
	for(int y = -int(this->WindowSize.y) / 2; y < int(this->WindowSize.y) / 2; y++)
	{
		printf("%2.3f\r", float(Count) / float(Total) * 100.f);

		for(int x = -int(this->WindowSize.x) / 2; x < int(this->WindowSize.x) / 2; x++)
		{
			Ray.set_direction(glm::vec3(ModelView * glm::normalize(glm::vec4(float(x), float(y), -float(this->WindowSize.y), 0.0f))));
			Ray.set_position(glm::vec3(ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			glm::vec3 Color = this->trace(Ray, Depth);
			//Surface.add(, Color);

			Surface.store<glm::vec4>(glm::uvec2(x, y) + glm::uvec2(this->WindowSize / glm::uint(2)), 0, glm::vec4(Color, 1.0f));

			Count++;
		}
	}

	gli::texture2d SurfaceScaled = scale_clamp(Surface, 0.f, 1.f);
	gli::texture2d Export = gli::convert(gli::flip(SurfaceScaled), gli::FORMAT_RGBA8_UNORM_PACK8);
	gli::save(Export, Config.getFile());
}

void camera::shootAntiAliasing(glm::mat4 const& ModelView, int Depth, int Antialising)
{
	config& Config = config::instance();

	ray Ray;
	Ray.set_environment_index(1.0f);

	gli::texture2d Surface(gli::FORMAT_RGBA32_SFLOAT_PACK32, this->WindowSize, 1);

	std::vector<glm::vec2> AntialisingBias(Antialising);
	for(std::vector<glm::vec2>::size_type i = 0; i < AntialisingBias.size(); i++)
		AntialisingBias[i] = glm::gaussRand(glm::vec2(0.0f), glm::vec2(0.7f));

	std::size_t Total = glm::compMul(this->WindowSize) * AntialisingBias.size();
	std::size_t Count = 0;

	//#pragma omp parallel for
	for(int y = -int(this->WindowSize.y) / 2; y < int(this->WindowSize.y) / 2; y++)
	{
		printf("%2.3f\r", float(Count) / float(Total) * 100.f);

		for(int x = -int(this->WindowSize.x) / 2; x < int(this->WindowSize.x) / 2; x++)
		{
			glm::ivec2 WindowPosition(x, y);

			for(std::vector<glm::vec2>::size_type i = 0; i < AntialisingBias.size(); i++)
			{
				Ray.set_direction(glm::vec3(ModelView * glm::normalize(glm::vec4(glm::vec2(WindowPosition) + AntialisingBias[i], -float(this->WindowSize.y), 0.0f))));
				Ray.set_position(glm::vec3(ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
				glm::vec3 const ColorTrace = this->trace(Ray, Depth) / float(Antialising);

				glm::uvec2 const TexelCoord = glm::uvec2(x, y) + glm::uvec2(this->WindowSize / glm::uint(2));

				glm::vec4 const ColorLoad = Surface.load<glm::vec4>(TexelCoord, 0);
				Surface.store(TexelCoord, 0, ColorLoad + glm::vec4(ColorTrace, 1.0f));

				Count++;
			}
		}
	}

	gli::texture2d SurfaceScaled = scale_clamp(Surface, 0.f, 1.f);
	gli::texture2d Export = gli::convert(gli::flip(SurfaceScaled), gli::FORMAT_RGBA8_UNORM_PACK8);
	gli::save(Export, Config.getFile());
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
bool camera::checkAliasing(gli::texture2d const& Surface, adaptator const& Adaptator, int x, int y)
{
	gli::sampler2d<float> Sampler(Surface, gli::WRAP_CLAMP_TO_EDGE);

	config& Config = config::instance();

	if(x < 1 || x >= int(this->WindowSize.x) || y < 1 || y >= int(this->WindowSize.y))
		return false;

	glm::vec3 ColorMM = Sampler.texel_fetch(glm::uvec2(x - 1, y - 1), 0) / Adaptator.get_factor(glm::uvec2(x - 1, y - 1));
	glm::vec3 ColorM0 = Sampler.texel_fetch(glm::uvec2(x - 1, y + 0), 0) / Adaptator.get_factor(glm::uvec2(x - 1, y + 0));
	glm::vec3 ColorMP = Sampler.texel_fetch(glm::uvec2(x - 1, y + 1), 0) / Adaptator.get_factor(glm::uvec2(x - 1, y + 1));

	glm::vec3 Color0M = Sampler.texel_fetch(glm::uvec2(x + 0, y - 1), 0) / Adaptator.get_factor(glm::uvec2(x + 0, y - 1));
	glm::vec3 Color00 = Sampler.texel_fetch(glm::uvec2(x + 0, y + 0), 0) / Adaptator.get_factor(glm::uvec2(x + 0, y + 0));
	glm::vec3 Color0P = Sampler.texel_fetch(glm::uvec2(x + 0, y + 1), 0) / Adaptator.get_factor(glm::uvec2(x + 0, y + 1));

	glm::vec3 ColorPM = Sampler.texel_fetch(glm::uvec2(x + 1, y - 1), 0) / Adaptator.get_factor(glm::uvec2(x + 1, y - 1));
	glm::vec3 ColorP0 = Sampler.texel_fetch(glm::uvec2(x + 1, y + 0), 0) / Adaptator.get_factor(glm::uvec2(x + 1, y + 0));
	glm::vec3 ColorPP = Sampler.texel_fetch(glm::uvec2(x + 1, y + 1), 0) / Adaptator.get_factor(glm::uvec2(x + 1, y + 1));

	if(glm::any(glm::greaterThan(glm::abs(ColorMM - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	if(glm::any(glm::greaterThan(glm::abs(ColorM0 - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	if(glm::any(glm::greaterThan(glm::abs(ColorMP - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	if(glm::any(glm::greaterThan(glm::abs(Color0M - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	if(glm::any(glm::greaterThan(glm::abs(Color0P - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	if(glm::any(glm::greaterThan(glm::abs(ColorPM - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	if(glm::any(glm::greaterThan(glm::abs(ColorP0 - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	if(glm::any(glm::greaterThan(glm::abs(ColorPP - Color00), glm::vec3(Config.getAntiAliasingAccuracy()))))
		return true;
	return false;
}

void camera::shootAntiAliasingAdaptative(glm::mat4 const& ModelView, int Depth, int Antialising)
{
/*
	adaptator Adaptator(this->WindowSize);

	ray Ray;
	Ray.set_environment_index(1.0f);

	gli::texture2d Surface(gli::FORMAT_RGBA32_SFLOAT_PACK32, this->WindowSize, 1);

	glm::vec2 *pAnti = new glm::vec2[Antialising];
	for(int i = 0; i < Antialising; ++i)
	{
		pAnti[i].x = 0.0f;//glm::linearRand(0.0f, 1.0f) * 2.0f - 1.0f;
		pAnti[i].y = 0.0f;//glm::linearRand(0.0f, 1.0f) * 2.0f - 1.0f;
	}

	std::size_t Total = glm::compMul(this->WindowSize) * Antialising;
	std::size_t Count = 0;
	for(int i = 0; i < Antialising; i++)
	{
		for(int y = -int(this->WindowSize.y / 2); y < int(this->WindowSize.y / 2); y++)
		{
			printf("%2.3f\r", float(Count) / float(Total) * 100.f);

			for(int x = -int(this->WindowSize.x / 2); x < int(this->WindowSize.x / 2); x++)
			{
				int x_tmp = x + this->WindowSize.x / 2;
				int y_tmp = y + this->WindowSize.y / 2;
				bool bAnti = true;
				if(i > 0)
					bAnti = this->checkAliasing(Surface, Adaptator, x_tmp, y_tmp);
				if(bAnti)
				{
					// 05/02/2005 - A same value is used to depth and height.
					Ray.set_direction(glm::vec3(ModelView * glm::normalize(glm::vec4(float(x + pAnti[i].x), float(y + pAnti[i].y), -float(this->WindowSize.y), 0.0f))));
					Ray.set_position(glm::vec3(ModelView * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
					Surface.add(glm::uvec2(x_tmp, y_tmp), this->trace(Ray, Depth));
					Adaptator.add(glm::uvec2(x_tmp, y_tmp));
					Count++;
				}
			}
		}
	}

	for(glm::uint y = 0; y < this->WindowSize.y; y++)
	for(glm::uint x = 0; x < this->WindowSize.x; x++)
		Surface.div(glm::uvec2(x, y), Adaptator.get_factor(glm::uvec2(x, y)));

	delete[] pAnti;

	Surface.save_as(config::instance().getFile());
*/
}

void camera::rotateX(action const& Move)
{
	switch(Move)
	{
	case PLUS:
		this->Rotate.x += 5;
		break;
	case MINUS:
		this->Rotate.x -= 5;
		break;
	}
}

void camera::rotateZ(action const& Move)
{
	switch(Move)
	{
	case PLUS:
		this->Rotate.z += 5;
		break;
	case MINUS:
		this->Rotate.z -= 5;
		break;
	}
}

void camera::move(action const& Move)
{
	switch(Move)
	{
	case PLUS:
		this->MoveForward++;
		if(this->MoveForward > 24) 
			this->MoveForward = 24; 
		break;
	case MINUS:
		this->MoveForward--;
		if(this->MoveForward < 1) 
			this->MoveForward = 1; 
		break;
	}
}
