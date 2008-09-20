#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/GraphicsBuffer.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Sampler.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/RenderLayout.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/PerlinNoise.hpp>

#include <KlayGE/D3D9/D3D9RenderFactory.hpp>
#include <KlayGE/OpenGL/OGLRenderFactory.hpp>

#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4251 4275 4512 4702)
#endif
#include <boost/program_options.hpp>
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

#include "Electro.hpp"

using namespace std;
using namespace KlayGE;

namespace
{
	class RenderElectro : public RenderableHelper
	{
	public:
		RenderElectro()
			: RenderableHelper(L"Electro")
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();

			MathLib::PerlinNoise<float>& pn = MathLib::PerlinNoise<float>::Instance();

			int const XSIZE = 128;
			int const YSIZE = 32;
			int const ZSIZE = 32;
			float const XSCALE = 0.04f;
			float const YSCALE = 0.08f;
			float const ZSCALE = 0.08f;

			std::vector<uint8_t> turbBuffer;
			turbBuffer.reserve(XSIZE * YSIZE * ZSIZE);
			uint16_t min = 255, max = 0;
			for (int z = 0; z < ZSIZE; ++ z)
			{
				for (int y = 0; y < YSIZE; ++ y)
				{
					for (int x = 0; x < XSIZE; ++ x)
					{
						uint8_t t = static_cast<uint8_t>(127 * (1
							+ pn.tileable_turbulence(XSCALE * x, YSCALE * y, ZSCALE * z,
								XSIZE * XSCALE, YSIZE * YSCALE, ZSIZE * ZSCALE, 16)));
						if (t > max)
						{
							max = t;
						}
						if (t < min)
						{
							min = t;
						}

						turbBuffer.push_back(t);
					}
				}
			}
			for (uint32_t i = 0; i < XSIZE * YSIZE * ZSIZE; ++ i)
			{
				turbBuffer[i] = static_cast<uint8_t>((255 * (turbBuffer[i] - min)) / (max - min));
			}

			TexturePtr electro_tex = rf.MakeTexture3D(XSIZE, YSIZE, ZSIZE, 1, EF_L8, EAH_CPU_Write | EAH_GPU_Read);
			{
				Texture::Mapper mapper(*electro_tex, 0, TMA_Write_Only, 0, 0, 0, XSIZE, YSIZE, ZSIZE);
				uint8_t* data = mapper.Pointer<uint8_t>();
				for (uint32_t z = 0; z < ZSIZE; ++ z)
				{
					for (uint32_t y = 0; y < YSIZE; ++ y)
					{
						memcpy(data, &turbBuffer[z * XSIZE * YSIZE + y * XSIZE], XSIZE);
						data += mapper.RowPitch();
					}
					data += mapper.SlicePitch() - mapper.RowPitch() * YSIZE;
				}
			}

			technique_ = rf.LoadEffect("Electro.kfx")->TechniqueByName("Electro");
			*(technique_->Effect().ParameterByName("electroSampler")) = electro_tex;

			float3 xyzs[] =
			{
				float3(-0.8f, 0.8f, 0.5f),
				float3(0.8f, 0.8f, 0.5f),
				float3(-0.8f, -0.8f, 0.5f),
				float3(0.8f, -0.8f, 0.5f),
			};

			float3 texs[] =
			{
				float3(-1, 0, 0),
				float3(1, 0, 0),
				float3(-1, -1, 0),
				float3(1, -1, 0),
			};

			rl_ = rf.MakeRenderLayout();
			rl_->TopologyType(RenderLayout::TT_TriangleStrip);

			GraphicsBufferPtr pos_vb = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Write | EAH_GPU_Read);
			pos_vb->Resize(sizeof(xyzs));
			{
				GraphicsBuffer::Mapper mapper(*pos_vb, BA_Write_Only);
				std::copy(&xyzs[0], &xyzs[0] + sizeof(xyzs) / sizeof(xyzs[0]), mapper.Pointer<float3>());
			}
			GraphicsBufferPtr tex0_vb = rf.MakeVertexBuffer(BU_Static, EAH_CPU_Write | EAH_GPU_Read);
			tex0_vb->Resize(sizeof(texs));
			{
				GraphicsBuffer::Mapper mapper(*tex0_vb, BA_Write_Only);
				std::copy(&texs[0], &texs[0] + sizeof(texs) / sizeof(texs[0]), mapper.Pointer<float3>());
			}

			rl_->BindVertexStream(pos_vb, boost::make_tuple(vertex_element(VEU_Position, 0, EF_BGR32F)));
			rl_->BindVertexStream(tex0_vb, boost::make_tuple(vertex_element(VEU_TextureCoord, 0, EF_BGR32F)));

			box_ = MathLib::compute_bounding_box<float>(&xyzs[0], &xyzs[0] + sizeof(xyzs) / sizeof(xyzs[0]));
		}

		void OnRenderBegin()
		{
			float const t = std::clock() * 0.0002f;

			*(technique_->Effect().ParameterByName("y")) = t * 2;
			*(technique_->Effect().ParameterByName("z")) = t;
		}
	};

	bool ConfirmDevice()
	{
		RenderEngine& re = Context::Instance().RenderFactoryInstance().RenderEngineInstance();
		RenderDeviceCaps const & caps = re.DeviceCaps();
		if (caps.max_shader_model < 2)
		{
			return false;
		}
		return true;
	}
}

int main()
{
	ResLoader::Instance().AddPath("../../media/Common");
	ResLoader::Instance().AddPath("../../media/Electro");

	RenderSettings settings;
	SceneManagerPtr sm;

	{
		int octree_depth = 3;
		int width = 800;
		int height = 600;
		int color_fmt = 13; // EF_ARGB8
		bool full_screen = false;

		boost::program_options::options_description desc("Configuration");
		desc.add_options()
			("context.render_factory", boost::program_options::value<std::string>(), "Render Factory")
			("context.input_factory", boost::program_options::value<std::string>(), "Input Factory")
			("context.scene_manager", boost::program_options::value<std::string>(), "Scene Manager")
			("octree.depth", boost::program_options::value<int>(&octree_depth)->default_value(3), "Octree depth")
			("screen.width", boost::program_options::value<int>(&width)->default_value(800), "Screen Width")
			("screen.height", boost::program_options::value<int>(&height)->default_value(600), "Screen Height")
			("screen.color_fmt", boost::program_options::value<int>(&color_fmt)->default_value(13), "Screen Color Format")
			("screen.fullscreen", boost::program_options::value<bool>(&full_screen)->default_value(false), "Full Screen");

		std::ifstream cfg_fs(ResLoader::Instance().Locate("KlayGE.cfg").c_str());
		if (cfg_fs)
		{
			boost::program_options::variables_map vm;
			boost::program_options::store(boost::program_options::parse_config_file(cfg_fs, desc), vm);
			boost::program_options::notify(vm);

			if (vm.count("context.render_factory"))
			{
				std::string rf_name = vm["context.render_factory"].as<std::string>();
				if ("D3D9" == rf_name)
				{
					Context::Instance().RenderFactoryInstance(D3D9RenderFactoryInstance());
				}
				if ("OpenGL" == rf_name)
				{
					Context::Instance().RenderFactoryInstance(OGLRenderFactoryInstance());
				}
			}
			else
			{
				Context::Instance().RenderFactoryInstance(D3D9RenderFactoryInstance());
			}
		}
		else
		{
			Context::Instance().RenderFactoryInstance(D3D9RenderFactoryInstance());
		}

		settings.width = width;
		settings.height = height;
		settings.color_fmt = static_cast<ElementFormat>(color_fmt);
		settings.full_screen = full_screen;
		settings.ConfirmDevice = ConfirmDevice;
	}

	Electro app("Electro", settings);
	app.Create();
	app.Run();

	return 0;
}

Electro::Electro(std::string const & name, RenderSettings const & settings)
			: App3DFramework(name, settings)
{
}

void Electro::InitObjects()
{
	font_ = Context::Instance().RenderFactoryInstance().MakeFont("gkai00mp.kfont", 16);

	renderElectro_.reset(new RenderElectro);
}

uint32_t Electro::DoUpdate(uint32_t /*pass*/)
{
	RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());

	renderEngine.CurFrameBuffer()->Clear(FrameBuffer::CBM_Color | FrameBuffer::CBM_Depth, Color(0.2f, 0.4f, 0.6f, 1), 1.0f, 0);

	renderElectro_->AddToRenderQueue();

	std::wostringstream stream;
	stream << this->FPS();

	font_->RenderText(0, 0, Color(1, 1, 0, 1), L"Electro Effect");
	font_->RenderText(0, 18, Color(1, 1, 0, 1), stream.str());

	return App3DFramework::URV_Need_Flush | App3DFramework::URV_Finished;
}
