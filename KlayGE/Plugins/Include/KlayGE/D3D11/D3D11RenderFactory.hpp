// D3D11RenderFactory.hpp
// KlayGE D3D11渲染引擎抽象工厂 头文件
// Ver 3.8.0
// 版权所有(C) 龚敏敏, 2009
// Homepage: http://www.klayge.org
//
// 3.8.0
// 初次建立 (2009.1.30)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef KLAYGE_PLUGINS_D3D11_RENDER_FACTORY_HPP
#define KLAYGE_PLUGINS_D3D11_RENDER_FACTORY_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/RenderFactory.hpp>

namespace KlayGE
{
	class D3D11RenderFactory : public RenderFactory
	{
	public:
		D3D11RenderFactory();

		std::wstring const & Name() const;

		virtual TexturePtr MakeDelayCreationTexture1D(uint32_t width, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint) override;
		virtual TexturePtr MakeDelayCreationTexture2D(uint32_t width, uint32_t height, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint) override;
		virtual TexturePtr MakeDelayCreationTexture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint) override;
		virtual TexturePtr MakeDelayCreationTextureCube(uint32_t size, uint32_t num_mip_maps, uint32_t array_size,
			ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint) override;

		FrameBufferPtr MakeFrameBuffer();

		RenderLayoutPtr MakeRenderLayout();

		virtual GraphicsBufferPtr MakeDelayCreationVertexBuffer(BufferUsage usage, uint32_t access_hint,
			uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) override;
		virtual GraphicsBufferPtr MakeDelayCreationIndexBuffer(BufferUsage usage, uint32_t access_hint,
			uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) override;
		virtual GraphicsBufferPtr MakeDelayCreationConstantBuffer(BufferUsage usage, uint32_t access_hint,
			uint32_t size_in_byte, ElementFormat fmt = EF_Unknown) override;

		QueryPtr MakeOcclusionQuery();
		QueryPtr MakeConditionalRender();
		QueryPtr MakeTimerQuery();
		QueryPtr MakeSOStatisticsQuery() override;

		virtual FencePtr MakeFence() override;

		RenderTargetViewPtr Make1DRtv(TexturePtr const & texture, ElementFormat pf, int first_array_index, int array_size,
			int level) override;
		RenderTargetViewPtr Make2DRtv(TexturePtr const & texture, ElementFormat pf, int first_array_index, int array_size,
			int level) override;
		RenderTargetViewPtr Make2DRtv(TexturePtr const & texture, ElementFormat pf, int array_index, Texture::CubeFaces face,
			int level) override;
		RenderTargetViewPtr Make2DRtv(TexturePtr const & texture, ElementFormat pf, int array_index, uint32_t slice,
			int level) override;
		RenderTargetViewPtr Make3DRtv(TexturePtr const & texture, ElementFormat pf, int array_index, uint32_t first_slice,
			uint32_t num_slices, int level) override;
		RenderTargetViewPtr MakeCubeRtv(TexturePtr const & texture, ElementFormat pf, int array_index, int level) override;
		RenderTargetViewPtr MakeBufferRtv(GraphicsBufferPtr const & gbuffer, ElementFormat pf, uint32_t first_elem,
			uint32_t num_elems) override;

		DepthStencilViewPtr Make2DDsv(uint32_t width, uint32_t height, ElementFormat pf,
			uint32_t sample_count, uint32_t sample_quality) override;
		DepthStencilViewPtr Make1DDsv(TexturePtr const & texture, ElementFormat pf, int first_array_index, int array_size,
			int level) override;
		DepthStencilViewPtr Make2DDsv(TexturePtr const & texture, ElementFormat pf, int first_array_index, int array_size,
			int level) override;
		DepthStencilViewPtr Make2DDsv(TexturePtr const & texture, ElementFormat pf, int array_index, Texture::CubeFaces face,
			int level) override;
		DepthStencilViewPtr Make2DDsv(TexturePtr const & texture, ElementFormat pf, int array_index, uint32_t slice,
			int level) override;
		DepthStencilViewPtr Make3DDsv(TexturePtr const & texture, ElementFormat pf, int array_index, uint32_t first_slice,
			uint32_t num_slices, int level) override;
		DepthStencilViewPtr MakeCubeDsv(TexturePtr const & texture, ElementFormat pf, int array_index, int level) override;

		UnorderedAccessViewPtr Make1DUav(TexturePtr const & texture, ElementFormat pf, int first_array_index,
			int array_size, int level) override;
		UnorderedAccessViewPtr Make2DUav(TexturePtr const & texture, ElementFormat pf, int first_array_index,
			int array_size, int level) override;
		UnorderedAccessViewPtr Make2DUav(TexturePtr const & texture, ElementFormat pf, int array_index,
			Texture::CubeFaces face, int level) override;
		UnorderedAccessViewPtr Make2DUav(TexturePtr const & texture, ElementFormat pf, int array_index, uint32_t slice,
			int level) override;
		UnorderedAccessViewPtr Make3DUav(TexturePtr const & texture, ElementFormat pf, int array_index,
			uint32_t first_slice, uint32_t num_slices, int level) override;
		UnorderedAccessViewPtr MakeCubeUav(TexturePtr const & texture, ElementFormat pf, int array_index,
			int level) override;
		UnorderedAccessViewPtr MakeBufferUav(GraphicsBufferPtr const & gbuffer, ElementFormat pf,
			uint32_t first_elem, uint32_t num_elems) override;

		ShaderObjectPtr MakeShaderObject();

	private:
		virtual std::unique_ptr<RenderEngine> DoMakeRenderEngine() override;

		RenderStateObjectPtr DoMakeRenderStateObject(RasterizerStateDesc const & rs_desc, DepthStencilStateDesc const & dss_desc,
			BlendStateDesc const & bs_desc) override;
		SamplerStateObjectPtr DoMakeSamplerStateObject(SamplerStateDesc const & desc);

		virtual void DoSuspend() override;
		virtual void DoResume() override;

	private:
		D3D11RenderFactory(D3D11RenderFactory const & rhs);
		D3D11RenderFactory& operator=(D3D11RenderFactory const & rhs);
	};
}

#endif			// KLAYGE_PLUGINS_D3D11_RENDER_FACTORY_HPP
