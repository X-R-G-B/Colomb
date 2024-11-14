// https://stackoverflow.com/questions/59165392/c-how-to-make-precise-frame-rate-limit/59165784#59165784

#include <chrono>
#include <ratio>
#include <thread>

template <std::intmax_t FPS>
class FrameRateLimit {
    public:
        FrameRateLimit()
            : _time_between_frames {1}, // std::ratio<1, FPS> seconds
              _tp {std::chrono::steady_clock::now()}
        {
        }

        void sleep()
        {
            // add to time point
            _tp += _time_between_frames;

            // and sleep until that time point
            std::this_thread::sleep_until(_tp);
        }

    private:
        // a duration with a length of 1/FPS seconds
        std::chrono::duration<double, std::ratio<1, FPS>> _time_between_frames;

        // the time point we'll add to in every loop
        std::chrono::time_point<std::chrono::steady_clock, decltype(_time_between_frames)> _tp;
};
