class AnswersRequestsStatistic
{
    //...
private:
    //...
    FirstReport firstReport(Azimuth::fromDegrees(180.0));
    SecondReport secondReport(maxPackWidth + widthMargin);
    std::array<StatisticReport *, 2> statReports = {&firstReport, &secondReport};
    std::array<std::optional<CpStats>, 2> calculatedStats;
};


bool isAccountedAtFirstReport(const AnswersRequestsStatistic::CoordinatePointBase & cp)
{
    bool isIntersect = packIntersectNorth(cp.pack);
    return not isIntersect or (isIntersect and isAfterNorth(cp.azimuth));
}

bool isAccountedAtSecondReport(const AnswersRequestsStatistic::CoordinatePointBase & cp)
{
    bool isIntersect = packIntersectNorth(cp.pack);
    return not isIntersect or (isIntersect and isBeforeNorth(cp.azimuth));
}

struct CpStats
{
    uint cpCount;
    std::vector<Response> repliesFromCp;

    CpStats operator+(const CpStats & otherStats)
    {
        auto summedReplies = this->repliesFromCp;
        summedReplies.insert(summedReplies.end(),
                             otherStats.repliesFromCp.begin(), otherStats.repliesFromCp.end());
        return {this->cpCount + otherStats.cpCount, summedReplies};
    }

    CpStats operator+(const std::optional<CpStats> & otherStats)
    {
        return *this + otherStats.value_or(CpStats());
    }
};

class StatisticReport
{
public:
    using CoordinatePoints = std::list<AnswersRequestsStatistic::CoordinatePointBase>;

public:
    StatisticReport(Azimuth azEnd)
    {
        azEndReport = azEnd;
    }
    virtual ~StatisticReport() = default;

    bool isReady(Azimuth prevAz, Azimuth az, int currTurn) const
    {
        return currTurn != prevReportTurn and azEndReport.inRange(prevAz, az);
    }
    CpStats calculate(const CoordinatePoints & cps, int currTurn)
    {
        prevReportTurn = currTurn;
        return {countCp(cps), countReplies(cps)};
    }

    virtual uint countCp(const CoordinatePoints & cps)                       = 0;
    virtual std::vector<Response> countReplies(const CoordinatePoints & cps) = 0;
    virtual void clear(CoordinatePoints & cps)                               = 0;

protected:
    Azimuth azEndReport;
    std::optional<uint> prevReportTurn;
};

class FirstReport : public StatisticReport
{
public:
    FirstReport(Azimuth azEnd)
        : StatisticReport(azEnd)
    {
    }
    uint countCp(const CoordinatePoints & cps) override
    {
        return std::count_if(cps.begin(), cps.end(), isAccountedAtFirstReport);
    }
    std::vector<Response> countReplies(const CoordinatePoints & cps) override
    {
        std::vector<Response> replies;
        for (auto & cp : cps)
        {
            if (packIntersectNorth(cp.pack))
                std::copy_if(cp.pack.begin(), cp.pack.end(), std::back_inserter(replies), [](auto reply)
                             { return isAfterNorth(reply->raw.radio.azimuth); });
            else
                std::copy(cp.pack.begin(), cp.pack.end(), std::back_inserter(replies));
        }
        return replies;
    }
    void clear(CoordinatePoints & cps) override
    {
        cps.clear();
    }
};

class SecondReport : public StatisticReport
{
public:
    SecondReport(Azimuth azEnd)
        : StatisticReport(azEnd)
    {
    }
    uint countCp(const CoordinatePoints & cps) override
    {
        return std::count_if(cps.begin(), cps.end(), isAccountedAtSecondReport);
    }
    std::vector<Response> countReplies(const CoordinatePoints & cps) override
    {
        std::vector<Response> replies;
        for (auto & cp : cps)
        {
            if (not packIntersectNorth(cp.pack) and isBeforeNorth(cp.azimuth))
                std::copy(cp.pack.begin(), cp.pack.end(), std::back_inserter(replies));
            else
                std::copy_if(cp.pack.begin(), cp.pack.end(), std::back_inserter(replies), [](auto reply)
                             { return isBeforeNorth(reply->raw.radio.azimuth); });
        }
        return replies;
    }
    void clear(CoordinatePoints & cps) override
    {
        cps.remove_if([](auto cp)
                      { return not packIntersectNorth(cp.pack) and isBeforeNorth(cp.azimuth); });
    }
};

void AnswersRequestsStatistic::setAzimuth(const Azimuth & azimuth)
{
    static std::optional<Azimuth> prevAz;

    auto stat = std::find_if(statReports.begin(), statReports.end(),
                             [this, azimuth](StatisticReport * statReport)
                             {
                                 return statReport->isReady(prevAz.value_or(azimuth), azimuth, currTurnoverNumber);
                             });
    if (stat != statReports.end())
    {
        auto stats = (*stat)->calculate(cps, currTurnoverNumber);
        (*stat)->clear(cps);
    }

    if (std::all_of(calculatedStats.begin(), calculatedStats.end(),
                    [](std::optional<CpStats> stats)
                    { return stats.has_value(); }))
    {
        auto totalStats = std::accumulate(calculatedStats.begin(), calculatedStats.end(), CpStats {});
        defferedReport(totalStats);
        calculatedStats.fill(std::nullopt);
    }
    
    prevAz = azimuth;
}

//было ЦС=9, стало ЦС=4
//выбор правильного типа
//  замена пары bool и Azimuth, на optional<Azimuth>
//полиморфизм: выделение общей логики для устранения "дублирования" кода
