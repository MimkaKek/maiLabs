# include <iostream>
# include <map>
# include <vector>
# include <string>
# include <queue>

using std::string;
using std::map;
using std::vector;
using std::queue;
using std::cin;
using std::cout;
using std::endl;

class BorNode;
typedef map<const char, BorNode*> LinksMap;

class BorNode {
public:
    LinksMap links;
    BorNode *fail;
    BorNode *term;
    int out;

public:
    BorNode(BorNode *fail_node = NULL): fail(fail_node), term(NULL), out(-1) { }

    BorNode* getLink(const char c) const {
        LinksMap::const_iterator iter = links.find(c);
        if (iter != links.cend()) {
            return iter->second;
        }
        else {
            return nullptr;
        }
    }

    bool isTerminal() const 
    {
        return (out >= 0);
    }
};

class AhoCorasick {
public:
    typedef void (*Callback) (const char* substr);
    BorNode root;
    vector<string> words;
    BorNode* current_state;

public:

    void addString(const char* const str) {
        BorNode *current_node = &root;
        for(const char *cp = str; *cp; ++cp) {
            BorNode *child_node = current_node->getLink(*cp);
            if (!child_node) {
                child_node = new BorNode(&root);
                current_node->links[*cp] = child_node;
            }
            current_node = child_node;
        }
        current_node->out = words.size();
        words.push_back(str);
    }

    void init() 
    {
        queue<BorNode*> q;
        q.push(&root);
        while (!q.empty()) {
            cout << "Queue #" << q.size() << ":\n";
            BorNode* current_node = q.front();
            q.pop();
            for (LinksMap::const_iterator iter = current_node->links.cbegin(); iter != current_node->links.cend(); ++iter)
            {
                const char symbol = iter->first;
                cout << "Symbol - " << symbol << ";\n";
                BorNode* child = iter->second;

                BorNode *temp_node = current_node->fail;
                while (temp_node) {
                    BorNode *fail_candidate = temp_node->getLink(symbol);
                    if (fail_candidate) {
                        cout << "Linking " << symbol << ";\n";
                        child->fail = fail_candidate;
                        break;
                    }
                    temp_node = temp_node->fail;
                }

                if (child->fail->isTerminal()) {
                    child->term = child->fail;
                }
                else {
                    child->term = child->fail->term;
                }
                q.push(child);
            }
        }
    }

    void step(const char c) 
    {
        while (current_state) {
            BorNode *candidate = current_state->getLink(c);
            if (candidate) {
                current_state = candidate;
                return;
            }
            current_state = current_state->fail;
        }
        current_state = &root;
    }

    void printTermsForCurrentState() const {
        if (current_state->isTerminal()) {
            cout << "found substring " << words[current_state->out].c_str() << "\n";
        }
        BorNode *temp_node = current_state->term;
        while (temp_node) {
            cout << "found substring " << words[temp_node->out].c_str() << "\n"; 
            temp_node = temp_node->term;
        }
    }

    void search(const char* str) 
    {
        current_state = &root;
        for(; *str; ++str) {
            cout << *str << ':' << endl;
            step(*str);
            printTermsForCurrentState();
         }
    }
};

void print(const char* str)
{
    cout << "found substring " << str << "\n";
}

int main()
{
    AhoCorasick ak;

    ak.addString("testtttovaya_stroka!");
    ak.addString("testtttovaya_stroka");
    ak.addString("testtttovaya_strok");
    ak.addString("testtttovaya_stro");
    ak.addString("testtttovaya_str");
    ak.addString("testtttovaya_st");
    ak.addString("testtttovaya_s");
    ak.addString("testtttovaya_");
    ak.addString("testtttovaya");
    ak.addString("testtttovay");
    ak.addString("testtttova");
    ak.addString("testtttov");
    ak.addString("testttto");
    ak.addString("testttt");
    ak.addString("testtt");
    ak.addString("testt");
    ak.addString("test");
    ak.addString("tes");
    ak.addString("te");
    ak.addString("t");

    ak.init();

    ak.search("testtttovaya_stroka!");

    cin.get();

    return 0;
}
