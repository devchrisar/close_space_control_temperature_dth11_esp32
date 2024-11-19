#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float processNoise, float measurementNoise, float estimationError, float initialValue)
{
    Q = processNoise;
    R = measurementNoise;
    P = estimationError;
    X = initialValue;
}

float KalmanFilter::update(float measurement)
{
    // Predicción
    P += Q;

    // Medida de actualización
    K = P / (P + R);
    X = X + K * (measurement - X);
    P = (1 - K) * P;

    return X;
}
