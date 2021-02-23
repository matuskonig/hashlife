#ifndef CONTEXT_HPP
#define CONTEXT_HPP

//Singleton context struct valid for the instance of the application.
struct Context
{
    //Flag indicating whether new generations should be computed or the program is paused
    bool should_update = false;

    //Flag indicating whether the hash life algorhitm is enabled (multiple generations are computed at once)
    //or regular quad tree with hashing is used
    bool hash_life = false;

    static Context &instance()
    {
        static Context c;
        return c;
    }

    void toggle_should_update()
    {
        should_update = !should_update;
    }
    
    void toggle_hash_life()
    {
        hash_life = !hash_life;
    }
};

#endif
