// Camera.cpp
// KlayGE 摄像机类 实现文件
// Ver 3.10.0
// 版权所有(C) 龚敏敏, 2003-2010
// Homepage: http://klayge.sourceforge.net
//
// 3.10.0
// 支持Motion blur (2010.2.22)
//
// 2.0.0
// 初次建立(2003.10.1)
//
// 修改记录
//////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/RenderFactory.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/Math.hpp>

#include <KlayGE/Camera.hpp>

namespace KlayGE
{
	// 构造函数
	//////////////////////////////////////////////////////////////////////////////////
	Camera::Camera()
		: prev_view_mats_(Context::Instance().RenderFactoryInstance().RenderEngineInstance().NumMotionFrames()),
			prev_proj_mats_(Context::Instance().RenderFactoryInstance().RenderEngineInstance().NumMotionFrames())
	{
		// 设置观察矩阵的参数
		this->ViewParams(float3(0, 0, 0), float3(0, 0, 1), float3(0, 1, 0));

		// 设置投射矩阵的参数
		this->ProjParams(PI / 4, 1, 1, 1000);
	}

	// 设置摄像机的观察矩阵
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ViewParams(float3 const & eyePos, float3 const & lookat,
										float3 const & upVec)
	{
		// 设置观察矩阵的参数
		eyePos_		= eyePos;
		lookat_		= lookat;
		upVec_		= upVec;

		viewVec_ = MathLib::normalize(lookat_ - eyePos_);
		viewMat_ = MathLib::look_at_lh(eyePos_, lookat_, upVec);
	}

	// 设置摄像机的投射矩阵
	//////////////////////////////////////////////////////////////////////////////////
	void Camera::ProjParams(float FOV, float aspect, float nearPlane,
											float farPlane)
	{
		// 设置投射矩阵的参数
		FOV_		= FOV;
		aspect_		= aspect;
		nearPlane_	= nearPlane;
		farPlane_	= farPlane;

		projMat_ = MathLib::perspective_fov_lh(FOV, aspect, nearPlane, farPlane);
	}

	void Camera::Update()
	{
		prev_view_mats_.push_back(viewMat_);
		prev_proj_mats_.push_back(projMat_);
	}
}
