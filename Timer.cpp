#include "WinMin.h"
#include "Timer.h"

Timer::Timer()
{
    __int64 countsPerSec{};
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    m_SecondsPerCount = 1.0 / (double)countsPerSec;
}

//�Q�[�����Ԃ𓝌v
float Timer::TotalTime()const
{

    //��~���Ԃ̓m�[�J��
    if (m_Stopped)
    {
        return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
    }
    else
    {
        return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
    }
}

//�t���[���^�C�����擾
float Timer::DeltaTime()const
{
    return (float)m_DeltaTime;
}

//�Q�[�����Ԃ����Z�b�g
void Timer::Reset()
{
    __int64 currTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    m_BaseTime = currTime;
    m_PrevTime = currTime;
    m_StopTime = 0;
    m_PausedTime = 0;   
    m_Stopped = false;
}

void Timer::Start()
{
    __int64 startTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

    if (m_Stopped)
    {
        m_PausedTime += (startTime - m_StopTime);

        m_PrevTime = startTime;
        m_StopTime = 0;
        m_Stopped = false;
    }
}

void Timer::Stop()
{
    if (!m_Stopped)
    {
        __int64 currTime{};
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        m_StopTime = currTime;
        m_Stopped = true;
    }
}

//���t���C������
void Timer::Tick()
{
    if (m_Stopped)
    {
        m_DeltaTime = 0.0;
        return;
    }

    __int64 currTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    m_CurrTime = currTime;

    //�t���C���^�C������
    m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;

    m_PrevTime = m_CurrTime;

    if (m_DeltaTime < 0.0)
    {
        m_DeltaTime = 0.0;
    }
}

bool Timer::IsStopped() const
{
    return m_Stopped;
}

