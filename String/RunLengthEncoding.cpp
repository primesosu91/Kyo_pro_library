// ランレングス圧縮（Run Length Encoding）を行うライブラリ
//
// example:
//
// ABC 143 C - Slimes (5Q)
// https://atcoder.jp/contests/abc143/submissions/78373937
// 圧縮後の長さを求める
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 数列をランレングス圧縮する関数
// 時間計算量: O(N)
template<typename T = ll>
vector<pair<T, ll>> run_length_encoding(const vector<T>& v) {
    vector<pair<T, ll>> res;
    for (const auto& x : v) {
        if (res.empty() || res.back().first != x) {
            // 新しい要素が見つかった場合は新規ブロックとして追加
            res.push_back({x, 1});
        } else {
            // 直前の要素と同じ場合は連続長を加算
            res.back().second++;
        }
    }
    return res;
}

// 文字列をランレングス圧縮する関数
// 時間計算量: O(N)
vector<pair<char, ll>> run_length_encoding(const string& s) {
    vector<pair<char, ll>> res;
    for (char c : s) {
        if (res.empty() || res.back().first != c) {
            // 新しい文字が見つかった場合は新規ブロックとして追加
            res.push_back({c, 1});
        } else {
            // 直前の文字と同じ場合は連続長を加算
            res.back().second++;
        }
    }
    return res;
}

// ランレングス圧縮された配列データを元の数列に復元する関数
// 時間計算量: O(展開後のサイズ)
template<typename T = ll>
vector<T> run_length_decoding(const vector<pair<T, ll>>& rle) {
    vector<T> res;
    for (const auto& p : rle) {
        for (ll i = 0; i < p.second; ++i) {
            res.push_back(p.first);
        }
    }
    return res;
}

// ランレングス圧縮された文字列データを元の文字列に復元する関数
// 時間計算量: O(展開後のサイズ)
string run_length_decoding(const vector<pair<char, ll>>& rle) {
    string res = "";
    for (const auto& p : rle) {
        res.append(p.second, p.first);
    }
    return res;
}