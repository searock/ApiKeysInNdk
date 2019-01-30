//
// Created by Searock on 1/29/19.
//

#include <jni.h>

extern "C" {

JNIEXPORT jstring JNICALL
Java_net_searock_apikeysinndk_MainActivity_getApiKey(JNIEnv *env, jobject instance) {

    jclass contextWrapperClass = env->FindClass("android/content/ContextWrapper");
    jstring fakeKey = env->NewStringUTF("FakeKey:D");

    jmethodID getPackageManagerMId = env->GetMethodID(contextWrapperClass,
                                                      "getPackageManager",
                                                      "()Landroid/content/pm/PackageManager;");

    jobject packageManager = env->CallObjectMethod(instance, getPackageManagerMId);

    jmethodID getPackageNameMId = env->GetMethodID(contextWrapperClass, "getPackageName",
                                                   "()Ljava/lang/String;");

    //might get an error here as the method returns an object and not a string
    jstring packageName = (jstring) env->CallObjectMethod(instance, getPackageNameMId);

    jint GET_SIGNATURES = 0x00000040;

    jclass packageManagerClass = env->GetObjectClass(packageManager);

    jmethodID getPackageInfoMId = env->GetMethodID(packageManagerClass,
                                                   "getPackageInfo",
                                                   "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    jobject packageInfo = env->CallObjectMethod(packageManager, getPackageInfoMId,
                                                packageName, GET_SIGNATURES);

    jclass packageInfoClass = env->GetObjectClass(packageInfo);

    jfieldID signatureFId = env->GetFieldID(packageInfoClass, "signatures",
                                            "[Landroid/content/pm/Signature;");

    //if this does not work get jobject and then cast it to jobjectarray
    jobjectArray signatures = (jobjectArray) env->GetObjectField(packageInfo, signatureFId);

    jobject signature = env->GetObjectArrayElement(signatures, 0);


    jclass signatureClass = env->GetObjectClass(signature);

    jmethodID hashCodeMId = env->GetMethodID(signatureClass, "hashCode", "()I");

    jint signatureHash = env->CallIntMethod(signature, hashCodeMId);

    //jint signatureHash = 5555;

    jclass integerClass = env->FindClass("java/lang/Integer");
    jmethodID toStringMId = env->GetStaticMethodID(integerClass, "toString",
                                                   "(I)Ljava/lang/String;");
    jstring signatureHashString = (jstring) env->CallStaticObjectMethod(integerClass,
                                                                        toStringMId, signatureHash);

    jstring encryptedData = env->NewStringUTF("FdmKQIPDLdXViVvfO3zamA==");
    jclass stringClass = env->FindClass("java/lang/String");
    jmethodID getBytesMId = env->GetMethodID(stringClass, "getBytes", "()[B");

    jbyteArray keyBytes = (jbyteArray) env->CallObjectMethod(signatureHashString, getBytesMId);

    jclass secretKeySpecClass = env->FindClass("javax/crypto/spec/SecretKeySpec");
    jmethodID secretKeySpecConstructor = env->GetMethodID(secretKeySpecClass, "<init>",
                                                          "([BLjava/lang/String;)V");

    jstring blowfish = env->NewStringUTF("Blowfish");

    jobject secretKeySpec = env->NewObject(secretKeySpecClass, secretKeySpecConstructor,
                                           keyBytes, blowfish);

    jclass cipherClass = env->FindClass("javax/crypto/Cipher");
    jmethodID getInstanceMId = env->GetStaticMethodID(cipherClass, "getInstance",
                                                      "(Ljava/lang/String;)Ljavax/crypto/Cipher;");

    jobject cipher = env->CallStaticObjectMethod(cipherClass, getInstanceMId, blowfish);

    jfieldID decryptModeFID = env->GetStaticFieldID(cipherClass, "DECRYPT_MODE", "I");
    jint DECRYPT_MODE = env->GetStaticIntField(cipherClass, decryptModeFID);

    jmethodID initMId = env->GetMethodID(cipherClass, "init",
                                         "(ILjava/security/Key;)V");

    env->CallVoidMethod(cipher, initMId, DECRYPT_MODE, secretKeySpec);

    jclass base64Class = env->FindClass("android/util/Base64");
    jfieldID defaultFId = env->GetStaticFieldID(base64Class, "DEFAULT", "I");
    jint DEFAULT = env->GetStaticIntField(base64Class, defaultFId);

    jbyteArray encryptedStringBytes = (jbyteArray) env->CallObjectMethod(encryptedData,
                                                                         getBytesMId);
    jmethodID decodeMId = env->GetStaticMethodID(base64Class, "decode", "([BI)[B");
    jbyteArray encryptedBase64Bytes = (jbyteArray) env->CallStaticObjectMethod(base64Class,
                                         decodeMId, encryptedStringBytes, DEFAULT);

    jmethodID doFinalMId = env->GetMethodID(cipherClass, "doFinal", "([B)[B");

    jbyteArray decryptedBytes = (jbyteArray) env->CallObjectMethod(
            cipher, doFinalMId, encryptedBase64Bytes);

    jboolean flag = env->ExceptionCheck();

    if (flag) {
        env->ExceptionClear();
        return fakeKey;
    }

    jmethodID stringConstructor = env->GetMethodID(stringClass, "<init>", "([B)V");

    jstring apiKey = (jstring) env->NewObject(stringClass, stringConstructor, decryptedBytes);

    return apiKey;

}


}

