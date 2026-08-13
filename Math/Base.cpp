// 10進数とN進数文字列・配列の相互変換ライブラリ
//
// example:
//
// ABC 336 C - Even Digits
// https://atcoder.jp/contests/abc336/submissions/me
// 5進数変換
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Base {
    // 文字列(base進数)を10進数の整数に変換する
    static ll base_to_dec(const string& s, ll base) {
        ll res = 0;
        for (char c : s) {
            res *= base;
            
            // 数字の場合の処理
            if ('0' <= c && c <= '9') {
                res += (c - '0');
            } 
            // 英大文字の場合の処理
            else if ('A' <= c && c <= 'Z') {
                res += (c - 'A' + 10);
            }
        }
        return res;
    }

    // 10進数の整数を文字列(base進数)に変換する
    static string dec_to_base(ll val, ll base) {
        // 0の場合は即座に返す
        if (val == 0) {
            return "0";
        }
        
        string res = "";
        while (val > 0) {
            ll rem = val % base;
            if (rem < 10) {
                res += (char)('0' + rem);
            } else {
                res += (char)('A' + (rem - 10));
            }
            val /= base;
        }
        
        // 下位桁から文字列に追加されているため反転する
        reverse(res.begin(), res.end());
        return res;
    }

    // from_base進数の文字列をto_base進数の文字列に直接変換する
    static string base_to_base(const string& s, ll from_base, ll to_base) {
        return dec_to_base(base_to_dec(s, from_base), to_base);
    }

    // 10進数の整数をbase進数分解し、下位桁から順に格納した配列を返す
    static vector<ll> dec_to_vec(ll val, ll base) {
        if (val == 0) {
            return {0};
        }
        
        vector<ll> res;
        while (val > 0) {
            res.push_back(val % base);
            val /= base;
        }
        return res;
    }

    // 下位桁から順に格納された配列(base進数)を10進数の整数に復元する
    static ll vec_to_dec(const vector<ll>& digits, ll base) {
        ll res = 0;
        ll mul = 1;
        for (ll d : digits) {
            res += d * mul;
            mul *= base;
        }
        return res;
    }
};