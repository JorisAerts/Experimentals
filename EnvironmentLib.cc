#include "EnvironmentLib.h"
#include <stdlib.h>
#if defined(WINDOWS) || defined(_WIN32)
#include <string>
#endif

struct JavaString
{
    JavaString(JNIEnv *env, jstring val):
        m_env(env),
        m_val(val),
        m_ptr(env->GetStringUTFChars(val, 0)) {}

    ~JavaString() {
        m_env->ReleaseStringUTFChars(m_val, m_ptr);
    }

    operator const char*() const {
        return m_ptr;
    }

    JNIEnv *m_env;
    jstring &m_val;
    const char *m_ptr;
};

JNIEXPORT jint JNICALL Java_EnvironmentLib_setenv
  (JNIEnv *env, jobject obj, jstring name, jstring value, jint overwrite)
{
    JavaString namep(env, name);
    JavaString valuep(env, value);
#if defined(WINDOWS) || defined(_WIN32)
    std::string s(namep);
    s += "=";
    s += valuep;
    int res = _putenv(s.c_str());
#else
    int res = setenv(namep, valuep, overwrite);
#endif
    return res;
}

JNIEXPORT jint JNICALL Java_EnvironmentLib_unsetenv
  (JNIEnv *env, jobject obj, jstring name)
{
    JavaString namep(env, name);
#if defined(WINDOWS) || defined(_WIN32)
    std::string s(namep);
    s += "=";
    int res = _putenv(s.c_str());
#else
    int res = unsetenv(namep);
#endif
    return res;
}
