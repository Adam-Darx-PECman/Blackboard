#include "computemodel.h"

ComputeModel::ComputeModel(QWidget *parent) : QWidget(parent)
{
    tb = new QTextBrowser(this);
    tb->setMinimumSize(this->size());
    auto font = QFont("Consolas");
    font.setItalic(true);
    font.setWeight(20);
    tb->setFont(font);

    indices = new QStringList;
    indices->append("\\[Mu]");
    indices->append("\\[Nu]");
    indices->append("\\[Rho]");
    indices->append("\\[Sigma]");
    indices->append("\\[Kappa]");
    indices->append("\\[Lambda]");
    indices->append("\\[Delta]");
    indices->append("\\[Tau]");
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    setMinimumSize(1000,100);
}

std::vector<std::string> split(const std::string& s, char delim = ' ') {
    std::vector<std::string> tokens;
    auto string_find_first_not = [s, delim](size_t pos = 0) -> size_t {
        for (size_t i = pos; i < s.size(); i++) {
            if (s[i] != delim) return i;
        }
        return std::string::npos;
    };
    size_t lastPos = string_find_first_not(0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != std::string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = string_find_first_not(pos);
        pos = s.find(delim, lastPos);
    }
    return tokens;
}

/*
 函数说明：对字符串中所有指定的子串进行替换
 参数：
std::string resource_str            //源字符串
std::string sub_str                //被替换子串
std::string new_str                //替换子串
返回值: std::string
 */
std::string subreplace(std::string resource_str, std::string sub_str, std::string new_str)
{
    std::string::size_type pos = 0;
    while((pos = resource_str.find(sub_str)) != std::string::npos)   //替换所有指定子串
    {
        resource_str.replace(pos, sub_str.length(), new_str);
    }
    return resource_str;
}


void ComputeModel::paintEvent(QPaintEvent*)
{
    tb->setMinimumSize(this->size());
}

double ComputeModel::direction(QPoint a, double angle)
{
    return a.x()*cos(angle)+a.y()*sin(angle);
}

bool ComputeModel::contains(QList<QPoint> *l, QPoint obj)
{
    for (auto item : *l)
        if((item.x() == obj.x())&&(item.y() == obj.y()))
            return true;
    return false;
}

double ComputeModel::distance(QPoint a, QPoint b)
{
    return sqrt((a.x()-b.x())*(a.x()-b.x()) + (a.y()-b.y())*(a.y()-b.y()));
}

int ComputeModel::findMaximumIn(QList<QPoint>* l, double angle)
{
    int index = 0;
    QPoint* p = new QPoint(0,0);
    for(int i = 0;i < l->size();i++)
        if(direction(*p,angle) < direction(l->at(i),angle))
        {
            p->setX(l->at(i).x());
            p->setY(l->at(i).y());
            index = i;
        }
    return index;
}

int ComputeModel::findMinimumIn(QList<QPoint>* l, double angle)
{
    int index = 0;
    QPoint* p = new QPoint(0,0);
    for(int i = 0;i < l->size();i++)
        if(direction(*p,angle) < direction(l->at(i),angle))
        {
            p->setX(l->at(i).x());
            p->setY(l->at(i).y());
            index = i;
        }
    return index;
}

void ComputeModel::on_compute(QList<QPoint>* s_begs, QList<QPoint>* s_ends, QList<QPoint>* w_begs, QList<QPoint>* w_ends, QList<QPoint>* dots)
{
    if(dots->empty())
        return;
    std::string str;

    double angle = 0;
    QPoint beg = dots->at(findMinimumIn(dots, angle));
    QPoint end = dots->at(findMaximumIn(dots, angle));

    QList<Particle> ingoing_electron;
    QList<Particle> outgoing_electron;
    QList<Particle> ingoing_positron;
    QList<Particle> outgoing_positron;
    QList<Particle> ingoing_photon;
    QList<Particle> outgoing_photon;
    QList<Particle> propagator_electron;
    QList<Particle> propagator_photon;
    auto vertex = *dots;      //默认全部顶角有效，后期可根据分析找到有效顶角，或提供删除/撤销功能，DATE:2020年7月23日

    for(int i = 0; i < s_begs->size();i++)
        if(!dots->contains(s_begs->at(i)))
        {
            if(direction(s_begs->at(i),angle) > direction(beg,angle))
                outgoing_positron.append(Particle("positron"+std::to_string(outgoing_positron.size()),s_begs->at(i),s_ends->at(i)));
            else if(direction(s_begs->at(i),angle) < direction(beg,angle))
                ingoing_electron.append(Particle("electron"+std::to_string(ingoing_electron.size()),s_begs->at(i),s_ends->at(i)));
        }
    for(int i = 0; i < s_ends->size();i++)
        if(!dots->contains(s_ends->at(i)))
        {
            if(direction(s_ends->at(i),angle) > direction(beg,angle))
                outgoing_electron.append(Particle("electron"+std::to_string(outgoing_electron.size()),s_begs->at(i),s_ends->at(i)));
            else if(direction(s_ends->at(i),angle) < direction(beg,angle))
                ingoing_positron.append(Particle("positron"+std::to_string(ingoing_positron.size()),s_begs->at(i),s_ends->at(i)));
        }
    for(int i = 0; i < s_begs->size();i++)
        if(dots->contains(s_begs->at(i)) && dots->contains(s_ends->at(i)))
            propagator_electron.append(Particle("electronpropagator"+std::to_string(propagator_electron.size()),s_begs->at(i),s_ends->at(i)));
    //重整光子列表 DATE:2020年7月23日
    for(int i = 0;i < w_begs->size();i++)
        if(direction(w_begs->at(i),angle) > direction(w_ends->at(i),angle))
        {
            QPoint* tmp = new QPoint(0,0);
            tmp->setX(w_begs->at(i).x());
            tmp->setY(w_begs->at(i).y());
            w_begs->replace(i,w_ends->at(i));
            w_ends->replace(i,*tmp);
        }

    for(int i = 0 ;i < w_begs->size();i++)
        if(!dots->contains(w_begs->at(i)))
            if(direction(w_begs->at(i),angle) < direction(beg,angle))
                ingoing_photon.append(Particle("photon"+std::to_string(ingoing_photon.size()),w_begs->at(i),w_ends->at(i)));
    for(int i = 0 ;i < w_ends->size();i++)
        if(!dots->contains(w_ends->at(i)))
            if(direction(w_ends->at(i),angle) > direction(end,angle))
                outgoing_photon.append(Particle("photon"+std::to_string(outgoing_photon.size()),w_begs->at(i),w_ends->at(i)));

    for(int i = 0; i < w_begs->size();i++)
        if(dots->contains(w_begs->at(i)) && dots->contains(w_ends->at(i)))
            propagator_photon.append(Particle("photonpropagator"+std::to_string(propagator_photon.size()),w_begs->at(i),w_ends->at(i)));
//    qDebug() << ingoing_photon.size() << endl;
//    qDebug() << ingoing_electron.size() << endl;
//    qDebug() << ingoing_positron.size() << endl;
//    qDebug() << outgoing_photon.size() << endl;
//    qDebug() << outgoing_electron.size() << endl;
//    qDebug() << outgoing_positron.size() << endl;
//    qDebug() << propagator_photon.size() << endl;
//    qDebug() << propagator_electron.size() << endl;
    //重新命名 DATE:2020年7月23日
    int offset = 0;
    for(int i = 0; i < ingoing_electron.size(); i++)
        ingoing_electron.replace(i,Particle("p"+std::to_string(i+1+offset),ingoing_electron.at(i).beg,ingoing_electron.at(i).end));
    offset += ingoing_electron.size();
    for(int i = 0;i < ingoing_positron.size(); i++)
        ingoing_positron.replace(i,Particle("p"+std::to_string(i+1+offset),ingoing_positron.at(i).beg,ingoing_positron.at(i).end));
    offset += ingoing_positron.size();
    for(int i = 0;i < ingoing_photon.size(); i++)
        ingoing_photon.replace(i,Particle("p"+std::to_string(i+1+offset),ingoing_photon.at(i).beg,ingoing_photon.at(i).end));
    offset += ingoing_photon.size();
    for(int i = 0; i < outgoing_electron.size(); i++)
        outgoing_electron.replace(i,Particle("p"+std::to_string(i+1+offset),outgoing_electron.at(i).beg,outgoing_electron.at(i).end));
    offset += outgoing_electron.size();
    for(int i = 0;i < outgoing_positron.size(); i++)
        outgoing_positron.replace(i,Particle("p"+std::to_string(i+1+offset),outgoing_positron.at(i).beg,outgoing_positron.at(i).end));
    offset += outgoing_positron.size();
    for(int i = 0;i < outgoing_photon.size(); i++)
        outgoing_photon.replace(i,Particle("p"+std::to_string(i+1+offset),outgoing_photon.at(i).beg,outgoing_photon.at(i).end));
    offset = 0;
    for(int i = 0; i < propagator_electron.size(); i++)
        propagator_electron.replace(i,Particle("q"+std::to_string(i+1+offset),propagator_electron.at(i).beg,propagator_electron.at(i).end));
    offset += propagator_electron.size();
    for(int i = 0;i < propagator_photon.size(); i++)
        propagator_photon.replace(i,Particle("q"+std::to_string(i+1+offset),propagator_photon.at(i).beg,propagator_photon.at(i).end));
    offset = 0;
    //组装表达式，遵循的费曼规则援引自《粒子物理导论》（第2版）格里菲斯
    //1.对入射粒子进行组装
    for(auto item : ingoing_electron)
        str = str+"(SpinorU"+"["+item.name+",SMP[\"m_e\"]])";
    for(auto item : ingoing_positron)
        str = str+"(SpinorV"+"["+item.name+",SMP[\"m_e\"]])";
    for(int i = 0;i < ingoing_photon.size();i++)
        str = str+"(Polarizationstd::vector["+ingoing_photon.at(i).name+","+indices->at(i).toStdString()+"])";
    //2.顶角因子
    for(int i = 0;i < vertex.size();i++)
        str = str+"(I*SMP[\"g\"]*"+"DiracMatrix["+indices->at(i).toStdString()+"])";
    //3.传播子
    for(auto item : propagator_electron)
        str = str+"(I*(DiracSlash["+item.name+"]+SMP[\"m_e\"]*c)/("+item.name+"^2-SMP[\"m_e\"]^2c^2)*1/(2\\[Pi])^4)";
    for(int i = 0; i < propagator_photon.size();i++)
        str = str+"(-I*MT["+indices->at(2*i).toStdString()+","+indices->at(2*i+1).toStdString()+"]/(" \
                +propagator_photon.at(i).name+"^2)*1/(2\\[Pi])^4)";
    //4.顶角处能量-动量守恒
    QList<std::string> deltas;
    for(auto p : vertex)
    {
        str = str+"((2\\[Pi])^4)";
        std::string tmp = "";
        for(auto item : ingoing_electron)
            if(distance(p,item.beg)==0)
                tmp = tmp + "(-" + item.name + ")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";
        for(auto item : ingoing_positron)
            if(distance(p,item.beg)==0)
                tmp = tmp+"(-"+item.name+")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";
        for(auto item : ingoing_photon)
            if(distance(p,item.beg)==0)
                tmp = tmp+"(-"+item.name+")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";

        for(auto item : outgoing_electron)
            if(distance(p,item.beg)==0)
                tmp = tmp+"(-"+item.name+")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";
        for(auto item : outgoing_positron)
            if(distance(p,item.beg)==0)
                tmp = tmp+"(-"+item.name+")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";
        for(auto item : outgoing_photon)
            if(distance(p,item.beg)==0)
                tmp = tmp+"(-"+item.name+")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";
        for(auto item : propagator_electron)
            if(distance(p,item.beg)==0)
                tmp = tmp+"(-"+item.name+")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";
        for(auto item : propagator_photon)
            if(distance(p,item.beg)==0)
                tmp = tmp+"(-"+item.name+")+";
            else if(distance(p,item.end)==0)
                tmp = tmp+"("+item.name+")+";
        if(!tmp.empty())
            tmp.pop_back();
        deltas.append(tmp);
        //str.append("DiracDelta["+tmp+"])";
    }

    //5.组装出射粒子
    for(auto item : outgoing_electron)
        str = str + "(SpinorUBar" + "[" + item.name + ",m])";
    for(auto item : outgoing_positron)
        str = str + "(SpinorVBar" + "[" + item.name + ",m])";
    for(int i = 0;i < outgoing_photon.size();i++)
        str = str + "(Conjugate[Polarizationstd::vector[" + outgoing_photon.at(i).name + ","+indices->at(i+ingoing_photon.size()).toStdString()+"]])";

    //6.积掉内线动量(由于MMA限制，手动积掉Delta函数)
//    std::string tmp;
//    tmp.append("Integrate["+str+",");
//    for(auto item : propagator_electron)
//        tmp.append("{"+item.name+",-Infinity,Infinity},");
//    for(auto item : propagator_photon)
//        tmp.append("{"+item.name+",-Infinity,Infinity},");
//    tmp.pop_back();
//    tmp.append("]");
//    str = tmp;

    for(int i = 0; i < deltas.size();i++)
    {
        auto momenta = split(deltas.at(i),'+');
        for(int j = 0; j < ingoing_photon.size() + ingoing_electron.size() + ingoing_positron.size(); j++)
        {
            int index = 0;
            int FLAG = 0;
            for(int k = 0; k < (int)momenta.size(); k++)
                if(momenta.at(k) == "(q"+std::to_string(j)+")")
                {
                    FLAG = 1;
                    index = k;
                    break;
                }
                else if(momenta.at(k) == "(-q"+std::to_string(j)+")")
                {
                    FLAG = -1;
                    index = k;
                    break;
                }
            //组装内线动量表达式
            std::string internal_exp = "";
            switch(FLAG)
            {
            case -1:
            {
                for(int k = 0; k < (int)momenta.size(); k++)
                    if(k != index)
                        internal_exp.append(momenta.at(k)).append("+");
                internal_exp.pop_back();
                internal_exp = "(" + internal_exp +")";
            }break;
            case 1:
            {
                for(int k = 0; k < (int)momenta.size(); k++)
                    if(k != index)
                        internal_exp.append(momenta.at(k)).append("+");
                internal_exp.pop_back();
                internal_exp = "(-(" + internal_exp +"))";
            }break;
            default:break;
            }
            str = subreplace(str,"q"+std::to_string(j),internal_exp);
            for(int l = 0; l < deltas.size();l++)
                deltas[l] = subreplace(deltas[l],"q"+std::to_string(j),internal_exp);
        }
        deltas.replace(i,"");
    }
    str = "(I/(2\\[Pi])^4)"+str;
    QList<QList<Particle>> l;
    l.append(ingoing_electron);
    l.append(ingoing_positron);
    l.append(ingoing_photon);

    l.append(outgoing_electron);
    l.append(outgoing_positron);
    l.append(outgoing_photon);

    l.append(propagator_electron);
    l.append(propagator_photon);

    emit on_mark(l);
    tb->setText(QString().fromStdString(str));
}
