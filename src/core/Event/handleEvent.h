#include <SDL3/SDL.h>

class Event {
    private:
        SDL_Event event;

    public:
        Event();
        ~Event();

        SDL_Event GetEvent();
        void GetKey();
        void GetKeyDown();
        void GetMousePos();
};