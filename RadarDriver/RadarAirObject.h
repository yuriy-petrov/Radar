#pragma once

#include <AirObject.h>

/**
 * @brief RadarAirObject используется драйвером радара для имитации целей.
 * @note Цели движутся равномерно и прямолинейно от начальной до конечной позиции
 */
class RadarAirObject : public AirObject
{
public:
    /**
     * @brief RadarAirObject конструктор
     * @param id идентификатор цели
     * @param startPos начальная позиция цели
     * @param endPos конечная позиция цели
     * @param speed скорость цели
     */
    RadarAirObject( const QString & id, const QPointF & startPos, const QPointF & endPos, double speed );

    /**
     * @brief update вычисление новой позиции цели на моент времени
     * @param timeStamp момент времени для расчета пройденного пути
     */
    void update( const QDateTime & timeStamp );

    /**
     * @brief print печать ы std::out данных о цели
     */
    void print();

private:
    QPointF _speedVector; // вектор скорости
    QPointF _endPos;      // конечная позиция
};
