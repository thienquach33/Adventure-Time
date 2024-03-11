#ifndef TIMER_H
#define TIMER_H

const int TARGET_FPS = 90;
const double TARGET_DELTATIME = 2.0f;

class Timer {
    public : 
        void Tick();
        inline double getDeltaTime() { return m_DeltaTime; }
        inline static Timer* getInstance () { return s_Instance = (s_Instance != nullptr) ? s_Instance : new Timer(); }
    private : 
        Timer() {}
        static Timer* s_Instance;
        double m_DeltaTime;
        double m_LastTime;
};

#endif // TIMER_H