//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#pragma once

class GPUResource
{
public:
	GPUResource()
		: m_GPUVirtualAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL),
		m_UsageState(D3D12_RESOURCE_STATE_COMMON),
		m_TransitionState((D3D12_RESOURCE_STATES)-1)
	{}

	GPUResource(ID3D12Resource* pResource, D3D12_RESOURCE_STATES currentState)
		: m_pResource(pResource),
		m_UsageState(currentState),
		m_TransitionState((D3D12_RESOURCE_STATES)-1),
		m_GPUVirtualAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL)
	{}

	~GPUResource() { Destroy(); }

	virtual void Destroy()
	{
		m_pResource = nullptr;
		m_GPUVirtualAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
		++m_VersionID;
	}

	ID3D12Resource* operator->() { return m_pResource.Get(); }
	const ID3D12Resource* operator->() const { return m_pResource.Get(); }

	ID3D12Resource* GetResource() { return m_pResource.Get(); }
	const ID3D12Resource* GetResource() const { return m_pResource.Get(); }

	ID3D12Resource** GetAddressOf() { return m_pResource.GetAddressOf(); }

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return m_GPUVirtualAddress; }

	uint32_t GetVersionID() const { return m_VersionID; }

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource;
	D3D12_RESOURCE_STATES m_UsageState;
	D3D12_RESOURCE_STATES m_TransitionState;
	D3D12_GPU_VIRTUAL_ADDRESS m_GPUVirtualAddress;

	// Used to identify when a resource changes so descriptors can be copied etc.
	uint32_t m_VersionID = 0;
};