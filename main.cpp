#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
using ll=long long;
using ld=long double;
struct node{
    char data;
    ll cnt;
    node* left;
    node* right;
};
node* parse(const string &expr, int& curpos){
    vector<node*> curnodes{};
    while (curpos<(int)expr.size()){
        char term=expr[curpos];
        curpos++;
        if (term=='('){
            node* sub=parse(expr, curpos);
            if (sub!=nullptr) curnodes.push_back(sub);
        } else if (term==')') break;
        else{
            curnodes.push_back(new node{term, 1, nullptr, nullptr});
        }
    }
    if (!curnodes.size()) return nullptr;
    node* root=curnodes[0];
    for (int i=1;i<(int)curnodes.size();i++){
        root=new node{'@', 1, root, curnodes[i]};
    }
    return root;
}
void delete_subtree(node* root){
    if (root==nullptr) return;
    root->cnt--; if (!root->cnt) {
        delete_subtree(root->left);
        delete_subtree(root->right);
        delete(root);
    }
}
node* copy(node* root){
    if (root!=nullptr) root->cnt++;
    return root;
}
bool apply(node*& root){
    if (root==nullptr) return false;
    if (root->data=='@'){
        node* rtl=root->left;
        node* rtr=root->right;
        if (rtl->data=='I'){
            node* x=copy(rtr);
            delete_subtree(root);
            root=x;
            return true;
        }
        if (rtl->data=='@'){
            node* rtll=rtl->left;
            node* rtlr=rtl->right;
            if (rtll->data=='K'){
                node* x=copy(rtlr);
                delete_subtree(root);
                root=x;
                return true;
            }
            if (rtll->data=='@') {
                node* rtlll=rtll->left;
                node* rtllr=rtll->right;
                if (rtlll->data=='S'){
                    node* newl=new node{'@', 1, copy(rtllr), copy(rtr)};
                    node* newr=new node{'@', 1, copy(rtlr), copy(rtr)};
                    node* newroot=new node{'@', 1, newl, newr};
                    delete_subtree(root);
                    root=newroot;
                    return true;
                }     
            } 
        }
    }
    bool lcr=apply(root->left);
    if (lcr) return true;
    bool rcr=apply(root->right);
    if (rcr) return true;
    return false;
}
bool balanced(const string& expr){
    int d=0;
    for (char c:expr){
        if (c=='(') d++;
        if (c==')') d--;
        if (d<0) return false;
    }
    return d==0;
}
void tostring(string& res,  node* root){
    if (root==nullptr) return;
    if (root->data=='@'){
        tostring(res, root->left);
        res+=' ';
        if (root->right->data=='@') res+='(';
        tostring(res, root->right);
        if (root->right->data=='@') res+=')';
    } else {
        res+=root->data;
    }
}
ld combcount(node* root, unordered_map<node*, ld>& dp){
    if (root==nullptr) return 0;
    if (dp.find(root)!=dp.end()) return dp[root];
    if (root->data=='@') dp[root]=combcount(root->left, dp)+combcount(root->right, dp);
    else dp[root]=1;
    return dp[root];
}
string reduce(string& expr){
    cout<<"reducing "<<expr<<'\n';
    if (!balanced(expr)) return "err: brackets not balanced";
    int start=0;
    node* rt=parse(expr, start);
    string ret="";
    unordered_map<node*, ld> mp{};
    for (int i=1;apply(rt);i++){
        if (i%5000==0) {
            mp.clear(); cout<<"reached "<<i<<" reductions\ncombinators: "<<combcount(rt, mp)<<'\n';
        }
    }
    ret=""; tostring(ret, rt);
    delete_subtree(rt);
    return ret;
}
int main(){
    string expression="SSS(SS)SS";
    cout<<reduce(expression)<<'\n';
    return 0;
}