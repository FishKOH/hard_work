PrioritetArray * prioritetsByAz(Azimuth az)
{
    // 315-45 Prioritet0
    // 45-135 Prioritet1
    // 135-225 Prioritet3
    // 225-315 Prioritet4
    uint sectorNum = (az + Azimuth::fromDegrees(45.0)).toAzimuthCode<2>();

    const std::array<PrioritetArray *, 4> prioritets = {&Prioritet0, &Prioritet1, &Prioritet3, &Prioritet4};

    return prioritets[sectorNum];
}

void LabelLayer::calcFormularPosition(std::shared_ptr<AbstractCoordinatePoint> & OutObj)
{
    auto labelPoint = OutObj->label()->labelPoint();
    double xc       = scale * (labelPoint.x() / 1000);
    double yc       = scale * (labelPoint.y() / 1000);

    int indexOfX = static_cast<int>(std::round(xc / WiFormul));
    int indexOfY = static_cast<int>(std::round(yc / HeFormul));

    Azimuth az = Azimuth::fromRadians(OutObj->azimuth());

    PrioritetArray & azPrioritet = *prioritetsByAz(az);

    auto priorityPtr = std::find_if_not(std::begin(azPrioritet)+1, std::end(azPrioritet),
                                        [indexOfX, indexOfY, this](unsigned char priority)
                                        {
                                            int x = TablSmXY[priority].X;
                                            int y = TablSmXY[priority].Y;
                                            return findPosition(indexOfX + x, indexOfY + y);
                                        });

    if (priorityPtr != std::end(azPrioritet))
    {
        int x = TablSmXY[*priorityPtr].X;
        int y = TablSmXY[*priorityPtr].Y;
        FormularPosition pos;
        pos.x = indexOfX + x;
        pos.y = indexOfY + y;
        m_usedPos.push_back(pos);

        OutObj->label()->setSme(QPoint(indexOfX * WiFormul, indexOfY * HeFormul));
    }
}
