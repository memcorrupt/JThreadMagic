package rip.mem.jthreadmagic;

public class JThreadMagic {

    public static final boolean LOADED;
    private static native void stopThread0(Thread thread, Object exception);

    static {
        boolean isLoaded = false;

        try{
            System.loadLibrary("jthreadmagic");
            isLoaded = true;
        }catch(Exception ex){
            System.err.println("Cannot load JThreadMagic:");
            ex.printStackTrace();
        }

        LOADED = isLoaded;
    }

    public static final void stopThread(Thread thread){
        stopThread0(thread, new ThreadDeath());
    }

}