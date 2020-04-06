#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwIdxSize(rhs.m_dwIdxSize)

{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,	// ������ ������ ũ��
															0,					// ������ ����(���ڰ� 0�� ��� ���� ����)
															m_dwVtxFVF,			// �����ϴ� ���ؽ��� �Ӽ���
															D3DPOOL_MANAGED,	// �޸� Ǯ�� ����
															&m_pVB, 
															NULL), 
															E_FAIL);


	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,	// ������ �ε��� ������ ũ��
															0,					// ������ ����(���ڰ� 0�� ��� ���� ����)
															m_IdxFmt,			// �����ϴ� �ε����� �Ӽ���
															D3DPOOL_MANAGED,	// �޸� Ǯ�� ����
															&m_pIB,
															NULL),
															E_FAIL);

	

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	// ����̽� ��ġ�� ���ؽ� ���۸� ��ũ�ϴ� �Լ�
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);	// 3���� : �ﰢ���� ����


}

void Engine::CVIBuffer::Free(void)
{
	CResources::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

