#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

class DomainRadixTreeNode
{
  public:
    char *Name;
    char *Value;
    DomainRadixTreeNode *ParentNode;
    vector<DomainRadixTreeNode *> ChildNodes;

    void AddDomain(char *domain, char *value);
    DomainRadixTreeNode *MatchesDomain(char *domain);
    int GetDefindDomainCount();
    char *GetDomainName();
    ~DomainRadixTreeNode();

    static DomainRadixTreeNode *CreateRoot();

  private:
    DomainRadixTreeNode(char *name, char *value, DomainRadixTreeNode *parent);
    char *GetDomainLastName(char *domain);
    char *GetDomainFirstName(char *domain);
    DomainRadixTreeNode *FindChild(char *name);
    DomainRadixTreeNode *FindDefinedParentDomain(DomainRadixTreeNode node);
};

DomainRadixTreeNode::DomainRadixTreeNode(char *name, char *value, DomainRadixTreeNode *parent)
{
    Name = name;
    Value = value;
    ParentNode = parent;
    //ChildNodes=new vector<DomainRadixTreeNode*>;
}

DomainRadixTreeNode::~DomainRadixTreeNode()
{
    //delete ChildNodes;
}
DomainRadixTreeNode *DomainRadixTreeNode::CreateRoot()
{
    return new DomainRadixTreeNode((char *)"/", (char *)"", NULL);
}

int FindLastIndexIn(char *find, char a)
{
    for (int i = strlen(find) - 1; i >= 0; i--)
    {
        if (find[i] == a)
        {
            return i;
        }
    }
    return -1;
}

char *Substr(char *srcstr, int offset, int length)
{

    int total_length = strlen(srcstr);

    int real_length = ((total_length - offset) >= length ? length : (total_length - offset)) + 1;
    char *tmp;
    if (NULL == (tmp = (char *)malloc(real_length * sizeof(char))))
    {
        printf("Memory overflow . \n");
    }
    strncpy(tmp, srcstr + offset, real_length - 1);
    tmp[real_length - 1] = '\0';

    return tmp;
}

char *DomainRadixTreeNode::GetDomainLastName(char *domain)
{
    char *ret = NULL;
    int lastDotIndex = FindLastIndexIn(domain, '.');
    int length = strlen(domain) - lastDotIndex - 1;
    ret = Substr(domain, lastDotIndex + 1, length);

    return ret;
}

char *DomainRadixTreeNode::GetDomainFirstName(char *domain)
{
    char *ret = NULL;

    int lastDotIndex = FindLastIndexIn(domain, '.');

    if (lastDotIndex >= 0)
    {
        ret = Substr(domain, 0, lastDotIndex);
    }
    return ret;
}

DomainRadixTreeNode *DomainRadixTreeNode::FindChild(char *name)
{
    //list<string> u;
    for (int i = 0; i < ChildNodes.size(); i++)
    {
        if (strcmp(ChildNodes[i]->Name, name) == 0)
        {
            return ChildNodes[i];
        }
    }
    return NULL;
}

DomainRadixTreeNode *DomainRadixTreeNode::FindDefinedParentDomain(DomainRadixTreeNode node)
{
    if (node.ParentNode == NULL)
    {
        return NULL;
    }
    else if (node.ParentNode->Value != "")
    {
        return node.ParentNode;
    }
    else
    {
        return FindDefinedParentDomain(*node.ParentNode);
    }
}

void DomainRadixTreeNode::AddDomain(char *domain, char *value)
{
    if (domain == NULL)
    {
        return;
    }
    char *dupDomain = strdup(domain);
    delete domain;
    char *lastName = GetDomainLastName(dupDomain);
    char *firstName = GetDomainFirstName(dupDomain);

    DomainRadixTreeNode *foundChild = FindChild(lastName);
    if (foundChild != NULL)
    {
        delete lastName;
        if (firstName == NULL)
        {
            foundChild->Value = value;
        }
        else
        {
            foundChild->AddDomain(firstName, value);
        }
    }
    else
    {
        char *childvalue = (char *)"";
        if (firstName == NULL)
        {
            childvalue = value;
        }
        DomainRadixTreeNode *child = new DomainRadixTreeNode(lastName, childvalue, this);
        child->AddDomain(firstName, value);
        ChildNodes.push_back(child);
    }
    delete dupDomain;
}

DomainRadixTreeNode *DomainRadixTreeNode::MatchesDomain(char *domain)
{
    DomainRadixTreeNode *ret = NULL;
    char *lastName = GetDomainLastName(domain);
    char *firstName = GetDomainFirstName(domain);
    DomainRadixTreeNode *childNode = FindChild(lastName);
    if (childNode != NULL && firstName != NULL)
    {
        ret = childNode->MatchesDomain(firstName);
    }
    else if (childNode != NULL && firstName == NULL && childNode->Value != "")
    {
        ret = childNode;
    }
    else if (childNode == NULL)
    {
        if (this->Value != "")
        {
            ret = this;
        }
        else
        {
            ret = FindDefinedParentDomain(*this);
        }
    }
    else
    {
        ret = NULL;
    }
    delete lastName, firstName;
    return ret;
}

int DomainRadixTreeNode::GetDefindDomainCount()
{
    int sum = 0;
    for (int i = 0; i < ChildNodes.size(); i++)
    {
        if (ChildNodes[i]->Value != NULL && ChildNodes[i]->Value != "")
        {
            sum++;
        }
        sum += ChildNodes[i]->GetDefindDomainCount();
    }
    return sum;
}

char *DomainRadixTreeNode::GetDomainName()
{
    char *ret = strdup(Name);
    if (ParentNode != NULL && strcmp(ParentNode->Name, (const char *)"/") != 0)
    {
        char *parentDomainName = ParentNode->GetDomainName();
        ret = new char[strlen(Name) + strlen(parentDomainName) + 2];
        ret[0] = '\0';
        strcat(ret, Name);
        strcat(ret, ".");
        strcat(ret, parentDomainName);

        delete parentDomainName;
    }
    return ret;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        cout << "Please specify adfree host." << endl;
        return 0;
    }
    DomainRadixTreeNode *root = DomainRadixTreeNode::CreateRoot();
    //root->AddDomain("google.com", "0.0.0.0");
    //root->AddDomain("map.google.com", "1.1.1.1");
    //root->AddDomain("jd.com", "2.2.2.2");
    cout << "Loading Domains from file: " << argv[1] << endl;
    ifstream *file = new ifstream(argv[1]);
    string *str = new string;
    time_t t0 = time(0);
    cout << ctime(&t0) << endl;
    while (getline(*file, *str))
    {
        int breakPos = str->find('/');
        //cout<<*str<<endl;
        char *domainName = Substr((char *)str->c_str(), 0, breakPos);
        char *ip = Substr((char *)str->c_str(), breakPos + 1, str->length() - breakPos - 1);
        //cout<<strDomainName<<"/"<<strIP<<endl;
        root->AddDomain(domainName, ip);
        delete str;
        str = new string;
    }
    delete file;
    time_t t1 = time(0);
    cout << ctime(&t1) << endl;
    cout << t1-t0 << endl;
    int count = root->GetDefindDomainCount();
    cout << "Domains Loaded: " << count << endl;
    //DomainRadixTreeNode *matchedNode = root->MatchesDomain("map.ad.jd.com");
    char *input = new char[255];
    while (cin >> input)
    {
        DomainRadixTreeNode *matchedNode = root->MatchesDomain(input);
        if (matchedNode != NULL)
        {
            cout << "matched: " << matchedNode->GetDomainName() << "/" << matchedNode->Value << endl;
        }
        else
        {
            cout << "no matched." << endl;
        }
        //delete matchedNode;
    }
    return 0;
}