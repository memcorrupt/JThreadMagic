#include <jni.h>
#include <jvmti.h>

jvmtiEnv *tiEnv = NULL;
jclass exceptionClass = NULL;

const jvmtiCapabilities neededCapabilities = {
    .can_signal_thread = 1
};

#define ASSERT_FOR_LOAD(condition, reason)                               \
    if(!(condition)){                                                    \
        fprintf(stderr, "JThreadMagic: failed to load -- " reason "!");  \
        return JNI_ERR;                                                  \
    }

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved){
    // Since JDK 1.2, only one JavaVM can be present per process, so it should be safe to fetch JVMTI env here
    // and save it until this library is unloaded. It *should* also be safe to store the ThreadDeath class and constructor.

    JNIEnv *env;
    ASSERT_FOR_LOAD((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_2) == JNI_OK, "cannot get JNI env");
    ASSERT_FOR_LOAD((*vm)->GetEnv(vm, (void **)&tiEnv, JVMTI_VERSION_1_2) == JNI_OK, "cannot get JVMTI env");

    ASSERT_FOR_LOAD((*tiEnv)->AddCapabilities(tiEnv, &neededCapabilities) == JVMTI_ERROR_NONE, "cannot add required JVMTI capabilities");

    jclass localExceptionClass = (*env)->FindClass(env, "java/lang/Exception");
    ASSERT_FOR_LOAD(localExceptionClass, "cannot find Exception class");
    exceptionClass = (*env)->NewGlobalRef(env, localExceptionClass);
    ASSERT_FOR_LOAD(exceptionClass, "cannot allocate global reference for Exception class");

    return JNI_VERSION_1_2;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved){
    if(tiEnv)
        (*tiEnv)->DisposeEnvironment(tiEnv);

    if(exceptionClass){
        JNIEnv *env;
        if((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_2) == JNI_OK)
            (*env)->DeleteGlobalRef(env, exceptionClass);
    }

    tiEnv = NULL;
    exceptionClass = NULL;
}

JNIEXPORT void JNICALL Java_rip_mem_jthreadmagic_JThreadMagic_stopThread0(JNIEnv *env, jclass clazz, jobject threadArgument, jobject exceptionArgument){
    jvmtiError err = (*tiEnv)->StopThread(tiEnv, threadArgument, exceptionArgument);

    // error handling
    if(err != JVMTI_ERROR_NONE && err != JVMTI_ERROR_THREAD_NOT_ALIVE){
        char *errorName;
        int hasErrorName = (*tiEnv)->GetErrorName(tiEnv, err, &errorName) == JVMTI_ERROR_NONE;

        if(!hasErrorName){
            errorName = "<unknown>";
        }

        char finalError[256];
        snprintf(
            finalError,
            sizeof(finalError), 
            "Cannot stop thread due to JVMTI error [%d]: %s",
            err,
            errorName
            );

        if(hasErrorName)
            (*tiEnv)->Deallocate(tiEnv, (unsigned char *)errorName);

        (*env)->ThrowNew(env, exceptionClass, finalError);
    }
}