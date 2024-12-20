package rip.mem.jthreadmagic;

public class JThreadMagic {

    public static final boolean LOADED;
    public static native void stopThread(Thread thread);

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

}