#ifndef _TIMESTEP_H
#define _TIMESTEP_H

namespace Hazel {

    class Timestep {
    public:
        Timestep(float time = 0.0f)
            : m_Time(time)
        {
        }

        float GetSeconds() const { return m_Time; }
        float GetMilisecond() const { return m_Time * 1000.0f; }

        operator float() const { return m_Time; }
    private:
        float m_Time;
    };

}

#endif // _TIMESTEP_H