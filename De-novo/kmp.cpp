#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// LPS(Longest Prefix Suffix) 배열 계산 함수
void computeLPSArray(const string& pattern, int M, vector<int>& lps) {
    int length = 0; // 현재 접두사와 접미사의 길이
    lps[0] = 0; // 첫 번째 값은 항상 0
    int i = 1;

    while (i < M) {
        if (pattern[i] == pattern[length]) { // 패턴의 문자가 일치하면
            length++;
            lps[i] = length;
            i++;
        }
        else {
            if (length != 0) {
                length = lps[length - 1]; // 이전 일치 길이를 사용
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// 두 문자열 간 최대 겹침 길이를 찾는 함수
int findOverlap(const string& text, const string& pattern, int max_length) {
    int N = text.size();
    int M = pattern.size();

    vector<int> lps(max_length, 0); // LPS 배열 생성
    computeLPSArray(pattern.substr(0, max_length), max_length, lps);

    int i = N - max_length; // 텍스트의 비교 시작 위치
    int j = 0; // 패턴의 비교 위치

    while (i < N) {
        if (text[i] == pattern[j]) { // 문자가 일치하면
            i++;
            j++;
        }

        if (j == max_length) { // 최대 겹침이 발견되면 반환
            return max_length;
        }
        else if (i < N && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1]; // LPS 값을 사용해 이동
            }
            else {
                i++;
            }
        }
    }
    return j; // 겹침 길이 반환
}


void kmpMerge(int pattern_length) {
    ifstream inputFile("reads.txt");
    if (!inputFile) {
        cerr << "reads.txt 파일 열기 실패" << endl;
        return;
    }

    vector<string> reads;
    string line;
    while (getline(inputFile, line)) {
        reads.push_back(line); // Read 데이터를 벡터에 저장
    }
    inputFile.close();

    string result = reads[0]; // 첫 번째 read를 결과로 초기화
    reads.erase(reads.begin()); // 첫 번째 read 제거

    while (!reads.empty()) { // 모든 read가 병합될 때까지 반복
        bool merged = false;

        for (auto it = reads.begin(); it != reads.end();) {
            int overlap = findOverlap(result, *it, pattern_length);

            if (overlap == pattern_length) { // 뒤로 병합
                result += it->substr(overlap);
                it = reads.erase(it); // 병합된 read 삭제
                merged = true;
                break;
            }

            overlap = findOverlap(*it, result, pattern_length);
            if (overlap == pattern_length) { // 앞으로 병합
                result = it->substr(0, it->size() - overlap) + result;
                it = reads.erase(it); // 병합된 read 삭제
                merged = true;
                break;
            }

            ++it; // 다음 read로 이동
        }
        if (!merged) {
            break;
        }
    }

    ofstream resultFile("result.txt");
    resultFile << result;
    resultFile.close();
}
void execute_KMP(int pattern_length) {
    clock_t start = clock();
    kmpMerge(pattern_length);
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "실행 시간: " << elapsed << "s" << std::endl;
    cout << "result.txt 생성 완료" << endl;
}