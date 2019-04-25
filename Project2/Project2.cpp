// Project2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

void GMM() {

	// Start Step (초기화)
	for (int i = 0; i < K; i++) { // 평균, 공분산, 사전확률 초기화
		u_1[i] = (double)(rand() % 256);
		u_2[i] = (double)(rand() % 256);
		u_1n[i] = u_1[i] / 255.0;
		u_2n[i] = u_1[i] / 255.0;

		s_1[i] = s_2[i] = 1.0;

		p[i] = 1.0 / (double)K;
		pre_like = like = 0.0;
	}

	// Expectation Step
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < data_count; j++) {
			pn[j] - 0.0;
			double tmp1, tmp2, tmp_p;
			double tmp_l = 0.0;

			tmp1 = pow((x_1n[j] - u_1n[i]), 2) / s_1[i];
			tmp1 = pow((x_2n[j] - u_2n[i]), 2) / s_2[i];

			pnk[j] = log(exp(-0.5 * (tmp1 + tmp2)));
			for (int k = 0; k < K; k++) {
				tmp1 = pow((x_1n[j] - u_1n[j]), 2) / s_1[k];
				tmp2 = pow((x_2n[j] - u_2n[j]), 2) / s_2[k];
				pn[j] += exp(-0.5 * (tmp1 + tmp2) * p[k]);
			}
			temp_l = 0.0;
			temp_l = log(pn[j]);
			if (!_finite(temp_l)) temp_l = 0.0;
			pn[j] = temp_l;
			tmp_p = log(p[i]);
			pkn[i][j] = exp(pnk[j] + tmp_p - pn[j]);
			pkn_t[i] += pkn[i][j];
		}
	}

	// Maximization Step
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < data_count; j++) {
			p[i] += pkn[i][j] / data_count; // 새로운 사전 확률 계산
			numer1 += pkn[i][j] * x_1n[j];
			numer2 += pkn[i][j] * x_2n[j];
			denom += pkn[i][j];
		}
		if (denom == 0) u_1n[i] = u_2n[i] = 0.0;
		else {
			u_1n[i] = numer1 / denom; // 새로운 평균 계산
			u_2n[i] = numer2 / denom;
		}
		numer1 = numer2 = denom = 0.0;
	}
	
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < data_count; j++) {
			numer1 += pkn[i][j] * pow((x_1n[j] - u_1n[i]), 2);
			numer2 += pkn[i][j] * pow((x_2n[j] - u_2n[i]), 2);
			denom += pkn[i][j];
		}
		if (denom == 0) s_1[i] = s_2[i] = 0.0;
		else {
			s_1[i] = numer1 / denom; // 새로운 분산 계산
			s_2[i] = numer2 / denom;
		}
		numer1 = numer2 = denom = 0.0;
	}

	// Repeat Steps Expectation to Maximization
	for (int i = 0; i < data_count; i++) {
		pn[i] = 0.0;
		double tmp1, tmp2;
		double temp_l = 0.0;

		for (int j = 0; j < K; j++) {
			tmp1 = pow((x_1n[i] - u_1n[j]), 2) / s_1[j];
			tmp2 = pow((x_2n[i] - u_2n[j]), 2) / s_2[j];
			pn[i] += exp(-0.5 * (tmp1 + tmp2)) * p[j];
		}

		temp_l = 0.0;
		temp_l = log(pn[i]);
		if (!_finite(temp_l)) temp_l = 0.0;
		pn[i] = temp_l;
		like += temp_l;
	}
	TRACE("rotation[%d], stop? = %f", cond + 1, fabs(like - pre_like));
	if (fabs(like - pre_like) < stop) {
		TRACE(" >>> stop! Training GMM End...\n");
		for (int i = 0; i < K; i++) {
			TRACE("P(%d) = %f u(%d) = [%f, %f], s(%d) = [[%f 0.000000][0.000000 %f]]\n", i, p[i], u_1n[i], u_2n[i], i, s_1[i], s_2[u]);
		}
		return;
	}

}


int main()
{
    std::cout << "Hello World!\n"; 
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
