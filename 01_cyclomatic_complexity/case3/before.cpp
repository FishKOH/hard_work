void AnswersRequestsStatistic::setAzimuth(const Azimuth &azimuth)
{
    static bool valueExist = false;
    static Azimuth prevAz;
    static int prevTurnFirstReport = -1;
    static int prevTurnSecondReport = -1;

    if (valueExist and Azimuth::fromDegrees(180.0).inRange(prevAz, azimuth))
    {
        if (prevTurnFirstReport != currTurnoverNumber)
        {
            prevTurnFirstReport = currTurnoverNumber;

            cpCnt += std::count_if(cps.begin(), cps.end(), [](auto cp)
            {
                bool isIntersect = packIntersectNorth(cp.pack);
                return not isIntersect or (isIntersect and isAfterNorth(cp.azimuth));
            });

            for (auto &cp : cps)
            {
                if (packIntersectNorth(cp.pack))
                    std::copy_if(cp.pack.begin(), cp.pack.end(), std::back_inserter(repliesFromCp),[](auto reply)
                    {
                        return isAfterNorth(reply->raw.radio.azimuth);
                    });
                else
                    std::copy(cp.pack.begin(), cp.pack.end(), std::back_inserter(repliesFromCp));
            }

            cps.clear();
        }
    }
    if (valueExist and (maxPackWidth+widthMargin).inRange(prevAz, azimuth))
    {
        if (prevTurnSecondReport != currTurnoverNumber)
        {
            prevTurnSecondReport = currTurnoverNumber;

            cpCnt += std::count_if(cps.begin(), cps.end(), [](auto cp)
            {
                bool isIntersect = packIntersectNorth(cp.pack);
                return not isIntersect or (isIntersect and isBeforeNorth(cp.azimuth));
            });

            for (auto &cp : cps)
            {
                if (not packIntersectNorth(cp.pack) and isBeforeNorth(cp.azimuth))
                    std::copy(cp.pack.begin(), cp.pack.end(), std::back_inserter(repliesFromCp));
                else
                    std::copy_if(cp.pack.begin(), cp.pack.end(), std::back_inserter(repliesFromCp),[](auto reply)
                    {
                        return isBeforeNorth(reply->raw.radio.azimuth);
                    });
            }

            cps.remove_if([](auto cp)
            {
                return not packIntersectNorth(cp.pack) and isBeforeNorth(cp.azimuth);
            });

            defferedReport();
            cpCnt = 0;
            repliesFromCp.clear();
        }
    }
    if (not valueExist)
        valueExist = true;
    prevAz = azimuth;
}
