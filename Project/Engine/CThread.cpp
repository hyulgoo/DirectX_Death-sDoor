#include "pch.h"

#include "CThread.h"
#include "CSync.h"

CThread::CThread() :
    m_Loop(false),
    m_Suspend(false)
{
}

CThread::~CThread()
{
    //Stop();

    DeleteCriticalSection(&m_CRT);
}

//쓰레드 일시정지
void CThread::Suspend()
{
    CSync   sync(&m_CRT);

    // 내부적으로 suspend count를 갖고 있는데, SuspendThread()를 호출하면 1 증가. ResumeThread()를 하면 1 감소. 0이 되어야지만 재시작이 됨. 
    SuspendThread(m_Thread);

    m_Suspend = true;
}

// 쓰레드 재시작
void CThread::Resume()
{
    CSync   sync(&m_CRT);

    DWORD Count = ResumeThread(m_Thread);

    if (Count > 0)
        m_Suspend = false;
}

// Suspend Count가 0이 될때까지 ResumeThread를 호출하여 무조건 재시작 시켜주는 함수.
void CThread::ReStart()
{
    CSync   sync(&m_CRT);

    DWORD   Count = 0;

    do
    {
        Count = ResumeThread(m_Thread);
    } while (Count > 0);

    m_Suspend = false;
}

void CThread::Stop()
{
    if (m_Thread)
    {
        m_Loop = false;
        Start();
        ReStart();

        // 스레드가 종료될때까지 기다린다.
        WaitForSingleObject(m_Thread, INFINITE);
        CloseHandle(m_Thread);
        m_Thread = 0;
    }
}

void CThread::Start()
{
    SetEvent(m_StartEvent);
}

bool CThread::Init()
{
    m_StartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    InitializeCriticalSection(&m_CRT);

    m_Thread = (HANDLE)_beginthreadex(nullptr, 0, CThread::ThreadFunction,
        (void*)this, 0, nullptr);

    return true;
}

unsigned int __stdcall CThread::ThreadFunction(void* Arg)
{
    CThread* Thread = (CThread*)Arg;

    // 지정된 개체
    WaitForSingleObject(Thread->m_StartEvent, INFINITE);

    do
    {
        Thread->Run();
    } while (Thread->m_Loop);

    return 0;
}
