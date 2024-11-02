#include <iostream> 
#include <string> 

using namespace std;

long long mod_pow(long long n, long long p, long long t)
{
    if (p < 0)
        return 0;
    n %= t;
    if (p == 0)
    {
        return 1;
    }
    if (p == 1)
    {
        return n;
    }
    if (p % 2 == 0)
    {
        return (mod_pow((n * n) % t, p / 2, t)) % t;
    }
    else
    {
        return (n * mod_pow((n * n) % t, p / 2, t)) % t;
    }
}

bool findCommon(string& a, string& b, long long len)
{
    long long j, d = len - a.size() - b.size();
    if (d >= 0)
        return false;
    d = -d;
    bool check{ true };
    if (d > b.size() || d > a.size())
        return false;
    for (j = 0; j < d && check; ++j)
    {
        if (a.at(a.size() - d + j) != b.at(j))
        {
            check = false;
        }
    }
    return check;
}


int main()
{
    bool com;
    long long t, len, mod, ans, i, j, space, d;
    string pre, post, sum1, sum2;
    cin >> t;
    for (i = 0; i < t; ++i)
    {
        ans = 0;
        cin >> len >> mod;
        cin >> pre >> post;
        space = len - (long long)pre.size() - (long long)post.size();
        if (space == 0)
        {
            ans = (post + pre == pre + post ? 1 : 2);
        }
        else if (space > 0)
        {
            sum1 = pre + post;
            sum2 = post + pre;


            if (post.size() != pre.size())
            {
                if (post.size() < pre.size())
                {
                    swap(pre, post);    //from this moment pre is shorter string 
                }

                com = true;
                for (j = 0; j < pre.size(); ++j)
                {
                    if (sum1.at(j) != sum2.at(j) || sum1.at(sum1.size() - j - 1) != sum2.at(sum1.size() - j - 1))
                    {
                        com = false;
                    }
                }

                ans = (2 * mod_pow(26, len - (long long)pre.size() - (long long)post.size(), mod)) % mod;

                if (com)
                {
                    if (space - (long long)post.size() + (long long)pre.size() < 0)
                    {
                        com = true; //again, the value of com is changing 
                        for (j = 0; j < (post.size() - pre.size() - space); ++j)
                        {
                            if (post.at(j) != post.at(pre.size() + space + j))
                            {
                                com = false;
                            }
                        }
                        if (com)
                        {
                            --ans;
                        }
                    }
                    else
                    {
                        ans -= mod_pow(26, space + (long long)pre.size() - (long long)post.size(), mod);
                    }
                }
            }
            else
            {
                ans = mod_pow(26, len - (long long)pre.size() - (long long)post.size(), mod) % mod;
                if (post != pre)
                {
                    ans = (ans * 2) % mod;
                }
            }
        }
        else
        {
            if (findCommon(pre, post, len) && findCommon(post, pre, len))
            {
                sum1 = pre;
                sum2 = post;
                for (j = -space; j < post.size(); ++j)
                {
                    sum1 += post.at(j);
                }
                for (j = -space; j < pre.size(); ++j)
                {
                    sum2 += pre.at(j);
                }
                ans = (sum1 == sum2 ? 1 : 2);
            }
            else if (findCommon(pre, post, len) || findCommon(post, pre, len))
            {
                ans = 1;
            }
        }

        if (ans < 0)
        {
            ans += 2 * mod;
            ans %= mod;
        }

        cout << ans % mod << '\n';
    }
}