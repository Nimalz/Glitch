#pragma once
#pragma once

#ifndef CPU_TIMER_H
#define CPU_TIMER_H

class Timer
{
public:
    Timer();

    float TotalTime()const;         //ゲーム時間を集計
    float DeltaTime()const;		    //フレイムタイム

    void Reset();                   //トータルタイムをリセット
    void Start();                   //再開
    void Stop();                    //休止
    void Tick();                    //毎フレイム呼ぶ
    bool IsStopped() const;         //休止中？

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