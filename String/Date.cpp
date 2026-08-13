// 日付計算ライブラリ: yyyy/mm/dd形式の文字列と経過日数の相互変換、加減算、差分計算、曜日取得など
//
// example:
//
// ARC 023 A - 経過日数 (7Q)
// https://atcoder.jp/contests/arc023/submissions/78336023
// day1からday2までの経過日数
//
// ARC 002 B - 割り切れる日付 (7Q)
// https://atcoder.jp/contests/arc002/submissions/78336185
// 条件を満たす日付までday++
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Date {
    ll y, m, d;
    ll days;

    // (y, m, d) を通算日数に変換 (1年1月1日を 0 とする)
    static ll to_days(ll y, ll m, ll d) {
        if (m <= 2) {
            y--;
            m += 12;
        }
        return y * 365 + y / 4 - y / 100 + y / 400 + 306 * (m + 1) / 10 + d - 429;
    }

    // 通算日数から (y, m, d) を復元
    static void from_days(ll days, ll &y, ll &m, ll &d) {
        ll z = days + 306;
        ll quad = z / 146097;
        z %= 146097;
        ll cent = z / 36524;
        if (cent == 4) cent = 3;
        z -= cent * 36524;
        ll quad_year = z / 1461;
        z %= 1461;
        ll year = z / 365;
        if (year == 4) year = 3;
        z -= year * 365;
        y = quad * 400 + cent * 100 + quad_year * 4 + year;
        m = (z * 5 + 2) / 153;
        d = z - (m * 153 + 2) / 5 + 1;
        m += 3;
        if (m > 12) {
            y++;
            m -= 12;
        }
    }

    // 年月日を指定して初期化
    Date(ll y, ll m, ll d) : y(y), m(m), d(d) {
        days = to_days(y, m, d);
    }

    // 経過日数から初期化
    Date(ll days_) : days(days_) {
        from_days(days, y, m, d);
    }

    // "yyyy/mm/dd" 等の文字列から初期化 (区切り文字は / や - など任意)
    Date(string s) {
        ll p1 = s.find_first_not_of("0123456789");
        ll p2 = s.find_first_not_of("0123456789", p1 + 1);
        y = stoll(s.substr(0, p1));
        m = stoll(s.substr(p1 + 1, p2 - p1 - 1));
        d = stoll(s.substr(p2 + 1));
        days = to_days(y, m, d);
    }

    // 指定した区切り文字で yyyy/mm/dd 形式の文字列を出力
    string to_string(char sep = '/') const {
        string ys = std::to_string(y);
        while (ys.size() < 4) ys = "0" + ys;
        string ms = std::to_string(m);
        if (ms.size() < 2) ms = "0" + ms;
        string ds = std::to_string(d);
        if (ds.size() < 2) ds = "0" + ds;
        return ys + sep + ms + sep + ds;
    }

    // d_days 日後の日付を取得
    Date add(ll d_days) const { return Date(days + d_days); }
    
    // 翌日の日付を取得
    Date next() const { return Date(days + 1); }
    
    // 前日の日付を取得
    Date prev() const { return Date(days - 1); }
    
    Date operator+(ll d_days) const { return add(d_days); }
    Date operator-(ll d_days) const { return add(-d_days); }

    // 対象日付 t との日数差 (this - t) を取得
    ll diff(const Date& t) const { return days - t.days; }
    ll operator-(const Date& t) const { return diff(t); }

    // 曜日を取得 (0: 日, 1: 月, ..., 6: 土)
    ll day_of_week() const { return ((days + 1) % 7 + 7) % 7; }
    
    // 曜日の文字列を取得
    string day_of_week_str() const {
        const vector<string> dow = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        return dow[day_of_week()];
    }

    // うるう年判定
    static bool is_leap(ll y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    // その月の最終日(28~31)を取得
    ll last_day_of_month() const {
        const vector<ll> m_days = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (m == 2 && is_leap(y)) return 29;
        return m_days[m];
    }

    // 実在する日付か判定
    static bool is_valid(ll y, ll m, ll d) {
        if (m < 1 || m > 12) return false;
        const vector<ll> m_days = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        ll max_d = m_days[m];
        if (m == 2 && is_leap(y)) max_d = 29;
        return 1 <= d && d <= max_d;
    }

    // 比較演算子
    bool operator==(const Date& t) const { return days == t.days; }
    bool operator!=(const Date& t) const { return days != t.days; }
    bool operator<(const Date& t) const { return days < t.days; }
    bool operator>(const Date& t) const { return days > t.days; }
    bool operator<=(const Date& t) const { return days <= t.days; }
    bool operator>=(const Date& t) const { return days >= t.days; }
};