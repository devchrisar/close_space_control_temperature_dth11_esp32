#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

class KalmanFilter
{
private:
    float Q; // Proceso de ruido de covarianza
    float R; // Medida de ruido de covarianza
    float P; // Error estimado
    float K; // Ganancia de Kalman
    float X; // Valor estimado

public:
    KalmanFilter(float processNoise, float measurementNoise, float estimationError, float initialValue);
    float update(float measurement);
};

#endif // KALMAN_FILTER_H
