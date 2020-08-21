#include <map>
#include <iostream>
#include <queue>
#include <random>
#include <chrono>

const bool PRINT = true;

class Timer {
private:
    // Псевдонимы типов используются для удобного доступа к вложенным типам
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    
    std::chrono::time_point<clock_t> m_beg;
 
public:
    Timer() : m_beg(clock_t::now())
    {
    }
    
    void reset()
    {
        m_beg = clock_t::now();
    }
    
    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};

long Min(long a, long b) {
    return a >= b ? b : a;
}

long Max(long a, long b) {
    return a >= b ? a : b;
}

class TEdge;

class TNode {
    public:

        TNode() {
            number = 0;
            parent = nullptr;
        };
        
        ~TNode() {};

        void connectNode(TEdge* edge, TNode* child) {
            int n = child->getNumber();
            cEdges[n] = edge;
            child->pEdges[this->number] = cEdges[n];
            return;
        };

        TEdge* getcEdge(int j) {
            auto it = cEdges.find(j);
            return (it != cEdges.end()) ? it->second : nullptr;
        };

        TEdge* getpEdge(int j) {
            auto it = pEdges.find(j);
            return (it != pEdges.end()) ? it->second : nullptr;
        };

        std::map<int, TEdge*>* getcEdges() {
            return &cEdges;
        }

        std::map<int, TEdge*>* getpEdges() {
            return &pEdges;
        }

        TNode* getParent() {
            return parent;
        };

        int getNumber() {
            return number;
        };

        void setParent(TNode* par) {
            parent = par;
            return;
        };
        
        void setNumber(int n) {
            number = n;
            return;
        };

        void printNode() {
            std::cout << "Node - " << number << ":" << std::endl;
            for(auto iter = cEdges.begin(); iter != cEdges.end(); ++iter) {
                std::cout << "\t" << "----> " << iter->first << std::endl;
            }
        };
        
    private:

        int                     number;
        
        std::map<int, TEdge*>   cEdges;
        std::map<int, TEdge*>   pEdges;

        TNode*                  parent;

};

class TEdge {
    public:

        TEdge () {
            curFlux = 0;
            maxFlux = 0;
            goForward = true;
            forPrint = true;
            inNode  = nullptr;
            outNode = nullptr;
        }
        
        TEdge (long flux, TNode* in, TNode* out) {
            curFlux = 0;
            maxFlux = flux;
            goForward = true;
            forPrint = true;
            inNode  = in;
            outNode = out;
        };

        ~TEdge() {};

        void init(long flux, TNode* in, TNode* out) {
            curFlux = 0;
            maxFlux = flux;
            goForward = true;
            forPrint = true;
            inNode  = in;
            outNode = out;
        };
        
        long getCurFlux() {
            return curFlux;
        };

        void incCurFlux(long delta) {

            if(goForward) {
                curFlux += delta;
            }
            else {
                curFlux -= delta;
            }

            goForward = true;
            return;
        };

        long getDiffFlux() {
            if(goForward) {
                return maxFlux - curFlux;
            }
            else {
                return curFlux;
            }
        };

        long getMaxFlux() {
            return maxFlux;
        };

        bool isFilled() {
            return curFlux == maxFlux;
        };

        TNode* getOutNode() {
            return outNode;
        };

        TNode* getInNode() {
            return inNode;
        };

        void setDirection(bool d) {
            goForward = d;
            forPrint = d;
            return;
        };

        void printEdge() {
            if(forPrint) {
                std::cout << "| " << inNode->getNumber() << " ---> " << outNode->getNumber() << " | "
                          << curFlux << "/" << maxFlux << " |" << std::endl;
            }
            else {
                std::cout << "| " << outNode->getNumber() << " ---> " << inNode->getNumber() << " | "
                          << curFlux << "/" << maxFlux << " |" << std::endl;
                forPrint = true;
            }

            return;
        };
    
    private:
        
        long    curFlux;
        long    maxFlux;
    
        bool    goForward;
        bool    forPrint;
        
        TNode*  inNode;
        TNode*  outNode;
};

TEdge** SearchPath(int n, TNode* nodes) {
    
    bool*  checked  = new bool[n];
    TNode* start    = nodes;
    TNode* node     = nullptr;
    TNode* tmp      = nullptr;
    
    std::queue<TNode*> toCheck;
    
    toCheck.push(nodes);
    checked[0] = true;

    while(!toCheck.empty()) {
        
        node = toCheck.front();
        toCheck.pop();
        
        for(auto iter = node->getcEdges()->begin(); iter != node->getcEdges()->end(); ++iter) {
            
            if(!checked[iter->first - 1] && !iter->second->isFilled()) {

                tmp = iter->second->getOutNode();
                tmp->setParent(node);

                if(n == iter->first) {
                    
                    unsigned int length = 0;
                    
                    while(tmp != start) {
                        tmp = tmp->getParent();
                        ++length;
                    }

                    TEdge** path = new TEdge*[length];
                    TEdge*  edge = nullptr;
                    
                    TNode* prev = node;
                    TNode* current = iter->second->getOutNode();

                    while(length > 0) {
                        edge = prev->getcEdge(current->getNumber());
                        if(edge == nullptr) {
                            edge = current->getcEdge(prev->getNumber());
                        }
                        
                        path[length - 1] = edge;

                        current = prev;
                        prev = prev->getParent();
                        
                        --length;
                    }

                    return path;
                }
                
                toCheck.push(tmp);
                checked[iter->first - 1] = true;
            }
        }

        for(auto iter = node->getpEdges()->begin(); iter != node->getpEdges()->end(); ++iter) {
            
            if(!checked[iter->first - 1] && iter->second->getCurFlux() > 0) {

                tmp = iter->second->getInNode();

                iter->second->setDirection(false);
                
                tmp->setParent(iter->second->getOutNode());
                
                toCheck.push(tmp);
                checked[iter->first - 1] = true;
            }
        }
    }


    delete[] checked;
    return nullptr;
}

void UpdateConnetions(bool* connections, TNode* nodes, int i, int j) {
    if(connections[i] && !(connections[j])) {
        connections[j] = true;
        auto cEdges = nodes[j].getcEdges();
        for(auto it = cEdges->begin(); it != cEdges->end(); ++it) {
            UpdateConnetions(connections, nodes, j, it->first - 1);
        }
    }

    return;
}

int main() {

    int attempts;
    int n = 0, m = 0, succesed = 0;
    double summary = 0;
    std::cin >> attempts;
    std::cin >> n;
    std::cin >> m;
    
    for(int att = 0; att < attempts; ++att) {

        int i = 0, j = 0, flux = 0;
        long pFlux = -1, tFlux = 0, mFlux = 0;
        
        TNode* nodes = new TNode[n];
        for(int pos = 0; pos < n; ++pos) {
            nodes[pos].setNumber(pos + 1);
        }

        TEdge* edges = new TEdge[m];
        bool*  connect = new bool[n];
        connect[0] = true;
        for(unsigned int i = 1; i < n; ++i) {
            connect[i] = false;
        }
        std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distr1(1, n);
        std::uniform_int_distribution<long> distr2(0,1000000000);
        for(int nstr = 0; nstr < m; ++nstr) {

            if(nstr == m - 1 && !connect[n - 1]) {
                j = n - 1;
                while(true) {
                    i = distr1(gen);
                    if(connect[i]) {
                        break;
                    }
                }
            }
            else {
                while(true) {
                    i = distr1(gen);
                    j = distr1(gen);
                    if(i != j) {
                        if(nodes[j - 1].getcEdge(i) == nullptr) {
                            break;
                        }
                    }
                }
            }
            
            //std::cout << "i = " << i << " | j = " << j << std::endl;
            flux = distr2(gen);

            edges[nstr].init(flux, nodes + i - 1, nodes + j - 1);
            //std::cout << "Connect" << std::endl;
            nodes[i - 1].connectNode(edges + nstr, nodes + j - 1);
            //std::cout << "Connections" << std::endl;
            UpdateConnetions(connect, nodes, i - 1, j - 1);
        }
        std::cout << "Start" << std::endl;
        Timer t;
        while(tFlux > pFlux) {
            pFlux = tFlux;
            if(PRINT) {
                std::cout << "--------------" << std::endl;
            }
            
            TEdge** path = SearchPath(n, nodes);
        
            if(path == nullptr) {
                if(PRINT) {
                    std::cout << "No path" << std::endl;
                }
                break;
            }
            else {
        
                int p = 0;
                mFlux = path[p]->getMaxFlux();
                while(path[p]->getOutNode() != (nodes + n - 1)) {
                    mFlux = Min(mFlux, path[p]->getDiffFlux());
                    ++p;
                }
                mFlux = Min(mFlux, path[p]->getDiffFlux());
        
                tFlux += mFlux;
        
                p = 0;
                while(path[p]->getOutNode() != (nodes + n - 1)) {
                    path[p]->incCurFlux(mFlux);
                    ++p;
                }
                path[p]->incCurFlux(mFlux);
                
                
                if(PRINT) {
                    p = 0;
                    while(path[p]->getOutNode() != (nodes + n - 1)) {
                        path[p]->printEdge();
                        ++p;
                    }
                    path[p]->printEdge();
                }
                
                
                
                delete[] path;
            }
        }

        
        if(PRINT) {
            std::cout << "--------------" << std::endl;
            std::cout << "Max Flux = " << tFlux << std::endl;
        }
        else {
            std::cout << tFlux << std::endl;
        }
        
        double res = t.elapsed();

        if(tFlux) {
            ++succesed;
            summary += res;
        }
        
        std::cout << "Time: " << res << " sec"<< std::endl;
        delete[] edges;
        delete[] nodes;
        delete[] connect;
    }
    std::cout << "Average time: " << summary / succesed << " sec" << std::endl;
    return 0;
}
