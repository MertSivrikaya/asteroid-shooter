#ifndef _UI_MANAGER_
#define _UI_MANAGER_

class UIManager
{
    public:
        UIManager(const UIManager & rhs) = delete;                // Deleting the copy constructor of the class so that compiler will not provide a default copy constructor.
        UIManager & operator=(const UIManager & rhs) = delete;    // Deleting assignement operator overloading of the class so that compiler will not provide a default one

        static UIManager & getInstance();                         // This method must be static as it will be called directly from the class

        void drawStartScreen();
        void drawPauseScreen();
        void drawEndGameScreen();

        void drawPlayerHealth();
        void drawPlayerScore();
        void drawPlayerCrosshair();

    private:
        UIManager();                                              // Making the default constructor private to prevent object construction         
};

#endif