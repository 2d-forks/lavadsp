#include "soundtouch/SoundTouch.h"

#include "jni.h"

using namespace soundtouch;

extern "C" {

#define METHOD(_RETURN, _NAME) JNIEXPORT _RETURN JNICALL Java_com_github_natanbc_timescale_natives_TimescaleLibrary_##_NAME
//see https://stackoverflow.com/questions/36298111/is-it-possible-to-use-sun-misc-unsafe-to-call-c-functions-without-jni/36309652#36309652
#define CRITICALMETHOD(_RETURN, _NAME) JNIEXPORT _RETURN JNICALL JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_##_NAME


	CRITICALMETHOD(jlong, create)(jint channels, jint sampleRate, jdouble speedRate) {
		auto st = new SoundTouch();
		st->setChannels((uint32_t)channels);
		st->setSampleRate((uint32_t)sampleRate);
		st->setTempo(speedRate);
		st->clear();
		return (jlong)st;
	}

	METHOD(jlong, create)(JNIEnv *env, jobject thiz, jint channels, jint sampleRate, jdouble speedRate) {
		return JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_create(channels, sampleRate, speedRate);
	}

	CRITICALMETHOD(void, destroy)(jlong instance) {
		delete (SoundTouch *)instance;
	}

	METHOD(void, destroy)(JNIEnv *env, jobject thiz, jlong instance) {
		JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_destroy(instance);
	}

	CRITICALMETHOD(jint, process)(jlong instance, jint unused1, jfloat* src, jint inputOffset, jint inputLength, jint unused2, jfloat* dest, jint outputOffset, jint outputLength, jint unused3, jint* written) {
		auto st = (SoundTouch *)instance;
		st->putSamples(src + inputOffset, (uint32_t)inputLength);
		uint32_t r = st->receiveSamples(dest + outputOffset, (uint32_t)outputLength);
		written[0] = (int32_t)r;
		return 0;
	}

	METHOD(jint, process)(JNIEnv *env, jobject thiz, jlong instance, jfloatArray input, jint inputOffset, jint inputLength,
		jfloatArray output, jint outputOffset, jint outputLength, jintArray written) {
		auto src = (jfloat *)env->GetPrimitiveArrayCritical(input, nullptr);
		auto dest = (jfloat *)env->GetPrimitiveArrayCritical(output, nullptr);
		auto w = (jint*)env->GetPrimitiveArrayCritical(written, nullptr);
		JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_process(instance, 0, src, inputOffset, inputLength, 0, dest, outputOffset, outputLength, 0, w);
		env->ReleasePrimitiveArrayCritical(input, src, JNI_ABORT);
		env->ReleasePrimitiveArrayCritical(output, dest, JNI_COMMIT);
		env->ReleasePrimitiveArrayCritical(written, w, JNI_COMMIT);
		return 0;
	}

	CRITICALMETHOD(jint, read)(jlong instance, jint unused, jfloat* dest, jint outputOffset, jint outputLength) {
		auto st = (SoundTouch*)instance;
		return (int32_t)st->receiveSamples(dest + outputOffset, (uint32_t)outputLength);
	}

	METHOD(jint, read)(JNIEnv* env, jobject thiz, jlong instance, jfloatArray output, jint outputOffset, jint outputLength) {
		auto dest = (jfloat *)env->GetPrimitiveArrayCritical(output, nullptr);
		auto j = (jint)JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_read(instance, 0, dest, outputOffset, outputLength);
		env->ReleasePrimitiveArrayCritical(output, dest, JNI_COMMIT);
		return j;
	}

	CRITICALMETHOD(void, reset)(jlong instance) {
		((SoundTouch*)instance)->clear();
	}

	METHOD(void, reset)(JNIEnv* env, jobject thiz, jlong instance) {
		JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_reset(instance);
	}

	CRITICALMETHOD(void, setSpeed)(jlong instance, jdouble speed) {
		((SoundTouch*)instance)->setTempo(speed);
	}

	METHOD(void, setSpeed)(JNIEnv* env, jobject thiz, jlong instance, jdouble speed) {
		JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_setSpeed(instance, speed);
	}

	CRITICALMETHOD(void, setPitch)(jlong instance, jdouble pitch) {
		((SoundTouch*)instance)->setPitch(pitch);
	}

	METHOD(void, setPitch)(JNIEnv* env, jobject thiz, jlong instance, jdouble pitch) {
		JavaCritical_com_github_natanbc_timescale_natives_TimescaleLibrary_setPitch(instance, pitch);
	}
}