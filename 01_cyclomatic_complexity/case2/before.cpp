void LabelLayer::calcFormularPosition(std::shared_ptr<AbstractCoordinatePoint>& OutObj)
{
    auto labelPoint = OutObj->label()->labelPoint();
    double xc = scale * (labelPoint.x() / 1000);
    double yc = scale * (labelPoint.y() / 1000);

    int Indi = static_cast<int>(std::round(yc / HeFormul));
    int Indj = static_cast<int>(std::round(xc / WiFormul));

    int Az = static_cast<int>(OutObj->azimuth() * 180 / M_PI);

    for (int k = 1; k <= 49; k++)
    {
        int i = 0, j = 0;
        if ((Az >= 225) && (Az < 315))
        {
            i = TablSmXY[Prioritet4[k]].X;
            j = TablSmXY[Prioritet4[k]].Y;
        }

        else if ((Az >= 315) || (Az < 45))
        {
            i = TablSmXY[Prioritet0[k]].X;
            j = TablSmXY[Prioritet0[k]].Y;
        }

        else if ((Az >= 45) && (Az < 135))
        {
            i = TablSmXY[Prioritet45[k]].X;
            j = TablSmXY[Prioritet45[k]].Y;
        }

        else if ((Az >= 135) && (Az < 225))
        {
            i = TablSmXY[Prioritet3[k]].X;
            j = TablSmXY[Prioritet3[k]].Y;
        }

        if (false == findPosition(Indj + i, Indi + j))
        {
            Indj += i;
            Indi += j;
            FormularPosition pos;
            pos.x = Indj;
            pos.y = Indi;
            m_usedPos.push_back(pos); //добавляем элемент

            OutObj->label()->setSme(QPoint(Indj * WiFormul, Indi * HeFormul));

            return;
        }
    }
}
