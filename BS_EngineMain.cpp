﻿#include "pch.h"
#include "BS_EngineMain.h"
#include "Common\DirectXHelper.h"

using namespace BS_Engine;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// 응용 프로그램이 로드되면 응용 프로그램 자산을 로드하고 초기화합니다.
BS_EngineMain::BS_EngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// 장치가 손실되었거나 다시 만들어지는 경우 알림을 등록합니다.
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: 이 항목을 앱 콘텐츠 초기화로 대체합니다.
	//m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));
	m_sceneRenderer = std::unique_ptr<SceneRenderer>(new SceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: 기본 가변 timestep 모드 외에 다른 설정을 하려면 타이머 설정을 변경합니다.
	// 예: 60FPS 고정 timestep 업데이트 논리일 경우 다음을 호출합니다.
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

BS_EngineMain::~BS_EngineMain()
{
	// 장치 알림 등록 취소
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// 창 크기가 변경되면 응용 프로그램 상태를 업데이트합니다(예: 장치 방향 변경).
void BS_EngineMain::CreateWindowSizeDependentResources() 
{
	// TODO: 이 항목을 앱 콘텐츠의 크기 종속 초기화로 대체합니다.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// 프레임당 한 번 응용 프로그램 상태를 업데이트합니다.
void BS_EngineMain::Update() 
{
	// 장면 개체를 업데이트합니다.
	m_timer.Tick([&]()
	{
		// TODO: 이 항목을 앱 콘텐츠 업데이트 함수로 대체합니다.
		//m_scene.Update(m_timer);
		m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

// 현재 응용 프로그램 상태에 따라 현재 프레임을 렌더링합니다.
// 프레임이 렌더링되어 표시할 준비가 되면 true를 반환합니다.
bool BS_EngineMain::Render() 
{
	// 처음 업데이트하기 전에 아무 것도 렌더링하지 마세요.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// 전체 화면을 대상으로 하도록 뷰포트를 다시 설정합니다.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// 렌더링 대상을 화면에 대해 다시 설정합니다.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// 백 버퍼 및 깊이 스텐실 뷰를 지웁니다.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 장면 개체를 렌더링합니다.
	// TODO: 이 항목을 앱 콘텐츠 렌더링 함수로 대체합니다.
	//m_scene.Render();
	m_sceneRenderer->Render();
	m_fpsTextRenderer->Render();

	return true;
}

// 릴리스가 필요한 장치 리소스를 렌더러에 알립니다.
void BS_EngineMain::OnDeviceLost()
{
	//m_scene.ReleaseDeviceDependentResources();
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// 장치 리소스가 이제 다시 만들어질 수 있음을 렌더러에 알립니다.
void BS_EngineMain::OnDeviceRestored()
{
	//m_scene.CreateDeviceDependentResources();
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
