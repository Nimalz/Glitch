#pragma once
#pragma once

#ifndef CPU_TIMER_H
#define CPU_TIMER_H

class Timer
{
public:
    Timer();

    float TotalTime()const;         //�Q�[�����Ԃ��W�v
    float DeltaTime()const;		    //�t���C���^�C��

    void Reset();                   //�g�[�^���^�C�������Z�b�g
    void Start();                   //�ĊJ
    void Stop();                    //�x�~
    void Tick();                    //���t���C���Ă�
    bool IsStopped() const;         //�x�~���H

private:
    double m_SecondsPerCount = 0.0;
    double m_DeltaTime = -1.0;

    __int64 m_BaseTime = 0;
    __int64 m_PausedTime = 0;
    __int64 m_StopTime = 0;
    __int64 m_PrevTime = 0;
    __int64 m_CurrTime = 0;

    bool m_Stopped = false;
};

#endif