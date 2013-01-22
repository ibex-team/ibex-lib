
#include "Quimper.h"
#include "IbexQuimper.h"

static ibex::Quimper * quimper = NULL;

JNIEXPORT jboolean JNICALL Java_Quimper_contract (JNIEnv *env, jobject, jstring ctc) {
  const char* _ctc = env->GetStringUTFChars(ctc,0);
  try {
    quimper->contract(_ctc);
  } catch(ibex::EmptyBoxException) {
    env->ReleaseStringUTFChars(ctc,_ctc);
    return false;
  }
  env->ReleaseStringUTFChars(ctc,_ctc);
  return true;
}

JNIEXPORT void JNICALL Java_Quimper_set_1domain (JNIEnv *env, jobject, jstring name, jdouble lb, jdouble ub) {
  const char* _name = env->GetStringUTFChars(name,0);
  quimper->set_domain(_name, lb, ub);
  env->ReleaseStringUTFChars(name,_name);
}

JNIEXPORT void JNICALL Java_Quimper_set_1var_1domain (JNIEnv *env, jobject, jint i, jdouble lb, jdouble ub) {
  quimper->set_var_domain(i,lb,ub);
}

JNIEXPORT void JNICALL Java_Quimper_set_1syb_1domain (JNIEnv *env, jobject, jint i, jdouble lb, jdouble ub) {
  quimper->set_syb_domain(i,lb,ub);
}

JNIEXPORT jdouble JNICALL Java_Quimper_get_1lb (JNIEnv *env, jobject, jstring name) {
  const char* _name = env->GetStringUTFChars(name,0);
  jdouble d=quimper->get_lb(_name);
  env->ReleaseStringUTFChars(name,_name);
  return d;
}

JNIEXPORT jdouble JNICALL Java_Quimper_get_1ub (JNIEnv *env, jobject, jstring name) {
  const char* _name = env->GetStringUTFChars(name,0);
  jdouble d=quimper->get_ub(_name);
  env->ReleaseStringUTFChars(name,_name);
  return d;
}

JNIEXPORT jdouble JNICALL Java_Quimper_get_1var_1lb (JNIEnv *env, jobject, jint i) {
  return quimper->get_var_lb(i);
}

JNIEXPORT jdouble JNICALL Java_Quimper_get_1var_1ub (JNIEnv *env, jobject, jint i) {
  return quimper->get_var_ub(i);
}

JNIEXPORT jdouble JNICALL Java_Quimper_get_1syb_1lb (JNIEnv *env, jobject, jint i) {
  return quimper->get_syb_lb(i);
}

JNIEXPORT jdouble JNICALL Java_Quimper_get_1syb_1ub (JNIEnv *env, jobject, jint i) {
  return quimper->get_syb_ub(i);
}

JNIEXPORT void JNICALL Java_Quimper_load (JNIEnv *env, jobject, jstring filename) {

  const char* _filename = env->GetStringUTFChars(filename,0);

  quimper = new ibex::Quimper(_filename);

  env->ReleaseStringUTFChars(filename,_filename);
}

JNIEXPORT void JNICALL Java_Quimper_release (JNIEnv *env, jobject) {
  delete quimper;
}


