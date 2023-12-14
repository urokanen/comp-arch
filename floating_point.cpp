#include <iostream>
#include <cstdlib>

using namespace std;

const unsigned int float_inf = 2139095040;
const unsigned int float_negative_inf = 4286578688;
const unsigned int float_nan = 4286578689;
const unsigned int float_zero = 0;
const unsigned int float_negative_zero = 2147483648;
const unsigned int half_inf = 31744;
const unsigned int half_nan = 31745;
const unsigned int half_negative_inf = 64512;
const unsigned int half_zero = 0;
const unsigned int half_negative_zero = 32768;

void error_message() {
    cerr << "incorrect input\n";
}

void error_number() {
    cerr << "incorrect number\n";
}

void error_operation() {
    cerr << "incorrect operation\n";
}

int float_type(int element) {
    unsigned int temp_exponent = (((unsigned int) element << 1) >> 24);
    int exponent = (int) temp_exponent - 127;
    unsigned int temp_mantissa = ((((unsigned int) element << 9) >> 9) << 1);
    int mantissa = (int) temp_mantissa;
    if (exponent == 128) {
        if (mantissa) {
            return 0; // nan
        } else if ((element >> 31)) {
            return 1; // -inf
        } else {
            return 2; // +inf
        }
    } else if (exponent == -127) {
        if (!mantissa) {
            if ((element >> 31)) {
                return 3; // -0
            } else {
                return 4; // 0
            }
        } else {
            return 5; // denormalize
        }
    } else {
        return 6;
    }
}

int half_type(int element) {
    unsigned int temp_exponent = (((unsigned int) element << 17) >> 27);
    int exponent = (int) temp_exponent - 15;
    unsigned int temp_mantissa = ((((unsigned int) element << 22) >> 22) << 2);
    int mantissa = (int) temp_mantissa;
    if (exponent == 16) {
        if (mantissa) {
            return 0;
        } else if ((element >> 15)) {
            return 1;
        } else {
            return 2;
        }
    } else if (exponent == -15) {
        if (!mantissa) {
            if ((element >> 15)) {
                return 3;
            } else {
                return 4;
            }
        } else {
            return 5;
        }
    } else {
        return 6;
    }
}

void form_output_float(int mantissa) {
    if (mantissa < 16) {
        cout << "00000" << hex << mantissa;
    } else if (mantissa < 256) {
        cout << "0000" << hex << mantissa;
    } else if (mantissa < 4096) {
        cout << "000" << hex << mantissa;
    } else if (mantissa < 65536) {
        cout << "00" << hex << mantissa;
    } else if (mantissa < 1048576) {
        cout << "0" << hex << mantissa;
    } else {
        cout << hex << mantissa;
    }
}

void form_output_half(int mantissa) {
    if (mantissa < 16) {
        cout << "00" << hex << mantissa;
    } else if (mantissa < 256) {
        cout << "0" << hex << mantissa;
    } else {
        cout << hex << mantissa;
    }
}

void print_float(int element) {
    if (!float_type(element)) {
        cout << "nan\n";
    } else if (float_type(element) == 1) {
        cout << "-inf\n";
    } else if (float_type(element) == 2) {
        cout << "inf\n";
    } else if (float_type(element) == 3) {
        cout << "-0x0.000000p+0\n";
    } else if (float_type(element) == 4) {
        cout << "0x0.000000p+0\n";
    } else {
        unsigned int temp_exponent = (((unsigned int) element << 1) >> 24);
        int exponent = (int) temp_exponent - 127;
        unsigned int temp_mantissa = ((((unsigned int) element << 9) >> 9) << 1);
        int mantissa = (int) temp_mantissa;
        if (float_type(element) == 5) {
            while (!((1 << (23)) & mantissa)) {
                mantissa = mantissa << 1;
                exponent--;
            }
            mantissa ^= (1 << (23));
            mantissa = mantissa << 1;
        }
        if ((element >> 31)) {
            cout << '-';
        }
        cout << "0x1.";
        form_output_float(mantissa);
        cout << 'p';
        if (exponent >= 0) {
            cout << '+';
        }
        cout << dec << exponent << "\n";
    }
}

void print_half(int element) {
    if (!half_type(element)) {
        cout << "nan\n";
    } else if (half_type(element) == 1) {
        cout << "-inf\n";
    } else if (half_type(element) == 2) {
        cout << "inf\n";
    } else if (half_type(element) == 3) {
        cout << "-0x0.000p+0\n";
    } else if (half_type(element) == 4) {
        cout << "0x0.000p+0\n";
    } else {
        unsigned int temp_exponent = (((unsigned int) element << 17) >> 27);
        int exponent = (int) temp_exponent - 15;
        unsigned int temp_mantissa = (((unsigned int) element << 22) >> 22);
        int mantissa = (int) temp_mantissa;
        if (half_type(element) == 5) {
            exponent++;
            while (!((1 << (10)) & mantissa)) {
                mantissa = mantissa << 1;
                exponent--;
            }
            mantissa ^= (1 << (10));
        }
        mantissa = mantissa << 2;
        if ((element >> 15)) {
            cout << '-';
        }
        cout << "0x1.";
        form_output_half(mantissa);
        cout << 'p';
        if (exponent >= 0) {
            cout << '+';
        }
        cout << dec << exponent << "\n";
    }
}

pair<int, int> float_number(int element) {
    unsigned int temp_exponent = (((unsigned int) element << 1) >> 24);
    int exponent = (int) temp_exponent - 127;
    unsigned int temp_mantissa = (((unsigned int) element << 9) >> 9);
    int mantissa = (int) temp_mantissa;
    if (exponent == -127) {
        exponent++;
        while (!((1 << (23)) & mantissa)) {
            mantissa = mantissa << 1;
            exponent--;
        }
    } else {
        mantissa ^= (1 << 23);
    }
    return {exponent, mantissa};
}

pair<int, int> half_number(int element) {
    unsigned int temp_exponent = (((unsigned int) element << 17) >> 27);
    int exponent = (int) temp_exponent - 15;
    unsigned int temp_mantissa = (((unsigned int) element << 22) >> 22);
    int mantissa = (int) temp_mantissa;
    if (exponent == -15) {
        exponent++;
        while (!((1 << (10)) & mantissa)) {
            mantissa = mantissa << 1;
            exponent--;
        }
    } else {
        mantissa ^= (1 << 10);
    }
    return {exponent, mantissa};
}

void make_equal(int &mantissa_1, int &exponent_1, int &mantissa_2, int &exponent_2, int &rounding) {
    while (exponent_1 > exponent_2) {
        if (mantissa_2 & 1) {
            rounding++;
        }
        mantissa_2 = mantissa_2 >> 1;
        exponent_2++;
    }
    while (exponent_2 > exponent_1) {
        if (mantissa_1 & 1) {
            rounding++;
        }
        mantissa_1 = mantissa_1 >> 1;
        exponent_1++;
    }
}

int result_float_sum(int mantissa_1, int mantissa_2, int exponent, int rounding, int element_1) {
    int result_mantissa = mantissa_1 + mantissa_2;
    if (rounding) {
        result_mantissa++;
        rounding = 0;
    }
    int result_exponent = exponent;
    if (result_mantissa & (1 << 24)) {
        result_mantissa = result_mantissa >> 1;
        result_exponent++;
    }
    while (result_exponent < -126) {
        if (result_mantissa & 1) {
            rounding = 1;
        }
        result_mantissa = result_mantissa >> 1;
        result_exponent++;
    }
    if (!(element_1 >> 31)) {
        result_mantissa += rounding;
        if (rounding && result_mantissa & (1 << 23)) {
            result_exponent++;
        }
    }
    if (result_exponent >= 128) {
        if (element_1 >> 31) {
            return (int) float_negative_inf;
        }
        return float_inf;
    }
    if (result_mantissa & (1 << 23)) {
        result_mantissa ^= (1 << 23);
    } else {
        result_exponent--;
    }
    unsigned int result = 0;
    if (element_1 >> 31) {
        result++;
    }
    result = result << 8;
    result_exponent += 127;
    result += result_exponent;
    result = result << 23;
    result += result_mantissa;
    return (int) result;
}

int result_half_sum(int mantissa_1, int mantissa_2, int exponent, int rounding, int element_1) {
    int result_mantissa = mantissa_1 + mantissa_2;
    if (rounding) {
        result_mantissa++;
        rounding = 0;
    }
    int result_exponent = exponent;
    if (result_mantissa & (1 << 11)) {
        result_mantissa = result_mantissa >> 1;
        result_exponent++;
    }
    while (result_exponent < -14) {
        if (result_mantissa & 1) {
            rounding = 1;
        }
        result_mantissa = result_mantissa >> 1;
        result_exponent++;
    }
    if (!(element_1 >> 15)) {
        result_mantissa += rounding;
        if (rounding && result_mantissa & (1 << 10)) {
            result_exponent++;
        }
    }
    if (result_exponent >= 16) {
        if (element_1 >> 15) {
            return half_negative_inf;
        }
        return half_inf;
    }
    if (result_mantissa & (1 << 10)) {
        result_mantissa ^= (1 << 10);
    } else {
        result_exponent--;
    }
    unsigned int result = 0;
    if (element_1 >> 15) {
        result++;
    }
    result = result << 5;
    result_exponent += 15;
    result += result_exponent;
    result = result << 10;
    result += result_mantissa;
    return (int) result;
}

int result_float_difference(int mantissa_1, int mantissa_2, int exponent, int rounding, int element_1) {
    int result_mantissa = mantissa_1 - mantissa_2;
    if (rounding) {
        result_mantissa--;
        rounding = 0;
    }
    int result_exponent = exponent;
    if (result_mantissa) {
        while (!(result_mantissa & (1 << 23))) {
            result_mantissa = result_mantissa << 1;
            result_exponent--;
        }
    } else {
        if (element_1 >> 31) {
            return (int) float_negative_zero;
        }
        return float_zero;
    }
    while (result_exponent < -126) {
        if (result_mantissa & 1) {
            rounding = 1;
        }
        result_mantissa = result_mantissa >> 1;
        result_exponent++;
    }
    if (!(element_1 >> 31)) {
        result_mantissa += rounding;
        if (rounding && ((1 << 23) & result_mantissa)) {
            result_exponent++;
        }
    }
    if (result_mantissa & (1 << 23)) {
        result_mantissa ^= (1 << 23);
    } else {
        result_exponent--;
    }
    if (result_exponent == -127 && !result_mantissa) {
        return float_zero;
    }
    unsigned int result = 0;
    if (element_1 >> 31) {
        result++;
    }
    result = result << 8;
    result_exponent += 127;
    result += result_exponent;
    result = result << 23;
    result += result_mantissa;
    return (int) result;
}

int result_half_difference(int mantissa_1, int mantissa_2, int exponent, int rounding, int element_1) {
    int result_mantissa = mantissa_1 - mantissa_2;
    if (rounding) {
        result_mantissa--;
        rounding = 0;
    }
    int result_exponent = exponent;
    if (result_mantissa) {
        while (!(result_mantissa & (1 << 10))) {
            result_mantissa = result_mantissa << 1;
            result_exponent--;
        }
    } else {
        if (element_1 >> 15) {
            return (int) half_negative_zero;
        }
        return half_zero;
    }
    while (result_exponent < -14) {
        if (result_mantissa & 1) {
            rounding = 1;
        }
        result_mantissa = result_mantissa >> 1;
        result_exponent++;
    }
    if (!(element_1 >> 15)) {
        result_mantissa += rounding;
        if (rounding && ((1 << 10) & result_mantissa)) {
            result_exponent++;
        }
    }
    if (result_mantissa & (1 << 10)) {
        result_mantissa ^= (1 << 10);
    } else {
        result_exponent--;
    }
    if (result_exponent == -15 && !result_mantissa) {
        return half_zero;
    }
    unsigned int result = 0;
    if (element_1 >> 15) {
        result++;
    }
    result = result << 5;
    result_exponent += 15;
    result += result_exponent;
    result = result << 10;
    result += result_mantissa;
    return (int) result;
}

int float_sum(int element_1, int element_2) {
    if (!float_type(element_1) || !float_type(element_2)) {
        return (int) float_nan;
    } else if (float_type(element_1) == 1 || float_type(element_2) == 1) {
        if (float_type(element_1) == 2 || float_type(element_2) == 2) {
            return (int) float_nan;
        } else {
            return (int) float_negative_inf;
        }
    } else if (float_type(element_1) == 2 || float_type(element_2) == 2) {
        if (float_type(element_1) == 2) {
            return float_inf;
        }
        return element_2;
    } else if (float_type(element_1) == 3 || float_type(element_2) == 3) {
        if (float_type(element_1) == 3) {
            return element_2;
        }
        return element_1;
    } else if (float_type(element_1) == 4 || float_type(element_2) == 4) {
        if (float_type(element_1) == 4) {
            return element_2;
        }
        return element_1;
    } else {
        auto number = float_number(element_1);
        int exponent_1 = number.first;
        int mantissa_1 = number.second;
        number = float_number(element_2);
        int exponent_2 = number.first;
        int mantissa_2 = number.second;
        int rounding = 0;
        make_equal(mantissa_1, exponent_1, mantissa_2, exponent_2, rounding);
        if ((element_1 >> 31) == (element_2 >> 31)) {
            if ((element_1 >> 31)) {
                rounding = 0;
            }
            return result_float_sum(mantissa_1, mantissa_2, exponent_1, rounding, element_1);
        } else {
            if (mantissa_1 < mantissa_2) {
                swap(element_1, element_2);
                swap(mantissa_1, mantissa_2);
            }
            if (!(element_1 >> 31)) {
                rounding = 0;
            }
            return result_float_difference(mantissa_1, mantissa_2, exponent_1, rounding, element_1);
        }
    }
}

int half_sum(int element_1, int element_2) {
    if (!half_type(element_1) || !half_type(element_2)) {
        return half_nan;
    } else if (half_type(element_1) == 1 || half_type(element_2) == 1) {
        if (half_type(element_1) == 2 || half_type(element_2) == 2) {
            return half_nan;
        } else {
            return half_negative_inf;
        }
    } else if (half_type(element_1) == 2 || half_type(element_2) == 2) {
        if (half_type(element_1) == 2) {
            return half_inf;
        }
        return element_2;
    } else if (half_type(element_1) == 3 || half_type(element_2) == 3) {
        if (half_type(element_1) == 3) {
            return element_2;
        }
        return element_1;
    } else if (half_type(element_1) == 4 || half_type(element_2) == 4) {
        if (half_type(element_1) == 4) {
            return element_2;
        }
        return element_1;
    } else {
        auto number = half_number(element_1);
        int exponent_1 = number.first;
        int mantissa_1 = number.second;
        number = half_number(element_2);
        int exponent_2 = number.first;
        int mantissa_2 = number.second;
        int rounding = 0;
        make_equal(mantissa_1, exponent_1, mantissa_2, exponent_2, rounding);
        if ((element_1 >> 15) == (element_2 >> 15)) {
            if ((element_1 >> 15)) {
                rounding = 0;
            }
            return result_half_sum(mantissa_1, mantissa_2, exponent_1, rounding, element_1);
        } else {
            if (mantissa_1 < mantissa_2) {
                swap(element_1, element_2);
                swap(mantissa_1, mantissa_2);
            }
            if (!(element_1 >> 31)) {
                rounding = 0;
            }
            return result_half_difference(mantissa_1, mantissa_2, exponent_1, rounding, element_1);
        }
    }
}

int float_difference(int element_1, int element_2) {
    element_2 ^= (1 << 31);
    return float_sum(element_1, element_2);
}

int half_difference(int element_1, int element_2) {
    element_2 ^= (1 << 15);
    return half_sum(element_1, element_2);
}

int float_multiplication(int element_1, int element_2) {
    if (!float_type(element_1) || !float_type(element_2)) {
        return (int) float_nan;
    } else if (float_type(element_1) == 1 || float_type(element_2) == 1) {
        if (float_type(element_1) == 2 || float_type(element_2) == 2) {
            return (int) float_nan;
        } else {
            return (int) float_negative_inf;
        }
    } else if (float_type(element_1) == 2 || float_type(element_2) == 2) {
        return float_inf;
    } else if (float_type(element_1) == 3 || float_type(element_2) == 3) {
        return (int) float_negative_zero;
    } else if (float_type(element_1) == 4 || float_type(element_2) == 4) {
        return float_zero;
    } else {
        auto number = float_number(element_1);
        int exponent_1 = number.first;
        int mantissa_1 = number.second;
        number = float_number(element_2);
        int exponent_2 = number.first;
        int mantissa_2 = number.second;
        int rounding = 0;
        unsigned long long result_mantissa = (unsigned long long) mantissa_1 * (unsigned long long) mantissa_2;
        int result_exponent = exponent_1 + exponent_2 - 23;
        unsigned long long mask = 1;
        mask = mask << 63;
        mask--;
        unsigned long long mask2 = 1;
        mask2 = mask2 << 24;
        mask2--;
        mask -= mask2;
        while (mask & result_mantissa) {
            if (result_mantissa & 1) {
                rounding = 1;
            }
            result_mantissa = result_mantissa >> 1;
            result_exponent++;
        }
        while (result_exponent < -126) {
            result_exponent++;
            if (result_exponent & 1) {
                rounding = 1;
            }
            result_mantissa = result_mantissa >> 1;
        }
        if (element_1 >> 31 == element_2 >> 31) {
            result_mantissa += rounding;
        }
        if (result_mantissa & (1 << 24)) {
            result_mantissa = result_mantissa >> 1;
            result_exponent++;
        }
        if (result_mantissa & (1 << 23)) {
            result_mantissa ^= (1 << 23);
        } else {
            result_exponent--;
        }
        if (result_exponent >= 128) {
            if (element_1 >> 31 == element_2 >> 31) {
                return float_inf;
            }
            return (int) float_negative_inf;
        } else if (!result_mantissa && result_exponent == -127) {
            return float_zero;
        }
        unsigned int result = 0;
        if (element_1 >> 31 != element_2 >> 31) {
            result++;
        }
        result = result << 8;
        result_exponent += 127;
        result += result_exponent;
        result = result << 23;
        result += result_mantissa;
        return (int) result;
    }
}

int half_multiplication(int element_1, int element_2) {
    if (!half_type(element_1) || !half_type(element_2)) {
        return half_nan;
    } else if (half_type(element_1) == 1 || half_type(element_2) == 1) {
        if (half_type(element_1) == 2 || half_type(element_2) == 2) {
            return half_nan;
        } else {
            return half_negative_inf;
        }
    } else if (half_type(element_1) == 2 || half_type(element_2) == 2) {
        return half_inf;
    } else if (half_type(element_1) == 3 || half_type(element_2) == 3) {
        return half_negative_zero;
    } else if (half_type(element_1) == 4 || half_type(element_2) == 4) {
        return half_zero;
    } else {
        auto number = half_number(element_1);
        int exponent_1 = number.first;
        int mantissa_1 = number.second;
        number = half_number(element_2);
        int exponent_2 = number.first;
        int mantissa_2 = number.second;
        int rounding = 0;
        unsigned long long result_mantissa = (unsigned long long) mantissa_1 * (unsigned long long) mantissa_2;
        int result_exponent = exponent_1 + exponent_2 - 10;
        unsigned long long mask = 1;
        mask = mask << 63;
        mask--;
        unsigned long long mask2 = 1;
        mask2 = mask2 << 11;
        mask2--;
        mask -= mask2;
        while (mask & result_mantissa) {
            if (result_mantissa & 1) {
                rounding = 1;
            }
            result_mantissa = result_mantissa >> 1;
            result_exponent++;
        }
        while (result_exponent < -14) {
            result_exponent++;
            if (result_exponent & 1) {
                rounding = 1;
            }
            result_mantissa = result_mantissa >> 1;
        }
        if (element_1 >> 15 == element_2 >> 15) {
            result_mantissa += rounding;
        }
        if (result_mantissa & (1 << 11)) {
            result_mantissa = result_mantissa >> 1;
            result_exponent++;
        }
        if (result_mantissa & (1 << 10)) {
            result_mantissa ^= (1 << 10);
        } else {
            result_exponent--;
        }
        if (result_exponent >= 16) {
            if (element_1 >> 15 == element_2 >> 15) {
                return half_inf;
            }
            return (int) half_negative_inf;
        } else if (!result_mantissa && result_exponent == -15) {
            return half_zero;
        }
        unsigned int result = 0;
        if (element_1 >> 15 != element_2 >> 15) {
            result++;
        }
        result = result << 5;
        result_exponent += 15;
        result += result_exponent;
        result = result << 10;
        result += result_mantissa;
        return (int) result;
    }
}

int float_division(int element_1, int element_2) {
    if (float_type(element_1) == float_type(element_2) && float_type(element_1) < 5) {
        return (int) float_nan;
    }
    if (!float_type(element_1) || !float_type(element_2)) {
        return (int) float_nan;
    } else if (float_type(element_1) == 1 || float_type(element_2) == 1) {
        if (float_type(element_1) == 2 || float_type(element_2) == 2) {
            return (int) float_nan;
        } else if (float_type(element_1) == 1) {
            return (int) float_negative_inf;
        }
        return (int) float_negative_zero;
    } else if (float_type(element_1) == 3 || float_type(element_2) == 3) {
        if (float_type(element_1) == 4 || float_type(element_2) == 4) {
            return (int) float_nan;
        } else if (float_type(element_1) == 3) {
            return (int) float_negative_zero;
        }
        return (int) float_negative_inf;
    } else if (float_type(element_1) == 2 || float_type(element_2) == 2) {
        if (float_type(element_1) == 2) {
            return float_inf;
        }
        return float_zero;
    } else if (float_type(element_1) == 4 || float_type(element_2) == 4) {
        if (float_type(element_1) == 4) {
            return float_zero;
        }
        return float_inf;
    } else {
        auto number = float_number(element_1);
        int exponent_1 = number.first;
        unsigned long long mantissa_1 = number.second;
        mantissa_1 = mantissa_1 << 23;
        number = float_number(element_2);
        int exponent_2 = number.first;
        unsigned long long mantissa_2 = number.second;
        int rounding = 0;
        unsigned long long result_mantissa = mantissa_1 / mantissa_2;
        int result_exponent = exponent_1 - exponent_2;
        if (mantissa_1 % mantissa_2) {
            rounding++;
        }
        if (element_1 >> 31 == element_2 >> 31) {
            result_mantissa += rounding;
            rounding = 0;
        }
        while (!(result_mantissa & (1 << 23))) {
            result_mantissa = result_mantissa << 1;
            result_exponent--;
        }
        while (result_exponent < -126) {
            result_exponent++;
            if (result_mantissa & 1) {
                rounding = 1;
            }
            result_mantissa = result_mantissa >> 1;
        }
        if (element_1 >> 31 == element_2 >> 31) {
            result_mantissa += rounding;
            if (rounding && result_mantissa & (1 << 23)) {
                result_exponent++;
            }
        }
        if (result_mantissa & (1 << 23)) {
            result_mantissa ^= (1 << 23);
        } else {
            result_exponent--;
        }
        if (result_exponent >= 128) {
            if (element_1 >> 31 == element_2 >> 31) {
                return float_inf;
            }
            return (int) float_negative_inf;
        } else if (!result_mantissa && result_exponent == -127) {
            return float_zero;
        }
        unsigned int result = 0;
        if (element_1 >> 31 != element_2 >> 31) {
            result++;
        }
        result = result << 8;
        result_exponent += 127;
        result += result_exponent;
        result = result << 23;
        result += result_mantissa;
        return (int) result;
    }
}

int half_division(int element_1, int element_2) {
    if (half_type(element_1) == half_type(element_2) && half_type(element_1) < 5) {
        return half_nan;
    }
    if (!half_type(element_1) || !half_type(element_2)) {
        return half_nan;
    } else if (half_type(element_1) == 1 || half_type(element_2) == 1) {
        if (half_type(element_1) == 2 || half_type(element_2) == 2) {
            return half_nan;
        } else if (half_type(element_1) == 1) {
            return half_negative_inf;
        }
        return half_negative_zero;
    } else if (half_type(element_1) == 3 || half_type(element_2) == 3) {
        if (half_type(element_1) == 4 || half_type(element_2) == 4) {
            return half_nan;
        } else if (half_type(element_1) == 3) {
            return half_negative_zero;
        }
        return half_negative_inf;
    } else if (half_type(element_1) == 2 || half_type(element_2) == 2) {
        if (half_type(element_1) == 2) {
            return half_inf;
        }
        return half_zero;
    } else if (half_type(element_1) == 4 || half_type(element_2) == 4) {
        if (half_type(element_1) == 4) {
            return half_zero;
        }
        return half_inf;
    } else {
        auto number = half_number(element_1);
        int exponent_1 = number.first;
        unsigned long long mantissa_1 = number.second;
        mantissa_1 = mantissa_1 << 10;
        number = half_number(element_2);
        int exponent_2 = number.first;
        unsigned long long mantissa_2 = number.second;
        int rounding = 0;
        unsigned long long result_mantissa = mantissa_1 / mantissa_2;
        int result_exponent = exponent_1 - exponent_2;
        if (mantissa_1 % mantissa_2) {
            rounding++;
        }
        if (element_1 >> 15 == element_2 >> 15) {
            result_mantissa += rounding;
            rounding = 0;
        }
        while (!(result_mantissa & (1 << 10))) {
            result_mantissa = result_mantissa << 1;
            result_exponent--;
        }
        while (result_exponent < -14) {
            result_exponent++;
            if (result_mantissa & 1) {
                rounding = 1;
            }
            result_mantissa = result_mantissa >> 1;
        }
        if (element_1 >> 15 == element_2 >> 15) {
            result_mantissa += rounding;
            if (rounding && result_mantissa & (1 << 10)) {
                result_exponent++;
            }
        }
        if (result_mantissa & (1 << 11)) {
            result_mantissa = result_mantissa >> 1;
            result_exponent++;
        }
        if (result_mantissa & (1 << 10)) {
            result_mantissa ^= (1 << 10);
        } else {
            result_exponent--;
        }
        if (result_exponent >= 16) {
            if (element_1 >> 15 == element_2 >> 15) {
                return half_inf;
            }
            return half_negative_inf;
        } else if (!result_mantissa && result_exponent == -15) {
            return half_zero;
        }
        unsigned int result = 0;
        if (element_1 >> 15 != element_2 >> 15) {
            result++;
        }
        result = result << 5;
        result_exponent += 15;
        result += result_exponent;
        result = result << 10;
        result += result_mantissa;
        return (int) result;
    }
}

void form_output_fixed(int element) {
    if (!element || element == 1000) {
        cout << "000\n";
    } else if (element < 10) {
        cout << "00" << element << "\n";
    } else if (element < 100) {
        cout << "0" << element << "\n";
    } else {
        cout << element << "\n";
    }
}

bool check_negative(int integer_part, int fractional_part, int element) {
    if (element & (1 << (integer_part + fractional_part - 1))) {
        return true;
    }
    return false;
}

int addition(int integer_part, int fractional_part, int element) {
    long long full_number = 1;
    full_number = full_number << (integer_part + fractional_part);
    long long result = full_number - element;
    return (int) result;
}

void print_fixed_point(int integer_part, int fractional_part, int element) {
    long long bitmask = 1;
    bitmask = (bitmask << (integer_part + fractional_part)) - 1;
    element &= (int) bitmask;
    if (check_negative(integer_part, fractional_part, element)) {
        element = addition(integer_part, fractional_part, element);
        int integer = element / (1 << fractional_part);
        long long fractional = (long long) element % (1 << fractional_part);
        fractional = (fractional * 1000) / (1 << fractional_part);
        cout << '-' << integer << '.';
        form_output_fixed(int(fractional));
    } else {
        int integer = element / (1 << fractional_part);
        long long fractional = (long long) element % (1 << fractional_part);
        long long result_fractional = (fractional * 1000) / (1 << fractional_part);
        if ((fractional * 1000) % (1 << fractional_part)) {
            result_fractional++;
        }
        cout << integer + result_fractional / 1000 << '.';
        form_output_fixed(int(result_fractional));
    }
}

int sum_fixed_point(int element_1, int element_2) {
    return element_1 + element_2;
}

int difference_fixed_point(int integer_part, int fractional_part, int element_1, int element_2) {
    element_2 = addition(integer_part, fractional_part, element_2);
    return element_1 + element_2;
}

int multiplication_fixed_point(int integer_part, int fractional_part, int element_1, int element_2) {
    int reverse = 0;
    if (check_negative(integer_part, fractional_part, element_1)) {
        element_1 = addition(integer_part, fractional_part, element_1);
        reverse++;
    }
    if (check_negative(integer_part, fractional_part, element_2)) {
        element_2 = addition(integer_part, fractional_part, element_2);
        reverse++;
    }
    long long result = (long long) element_1 * (long long) element_2;
    long long round = result & ((1 << fractional_part) - 1);
    result = (result >> fractional_part);
    if (reverse % 2 == 1) {
        result = addition(integer_part, fractional_part, (int) result);
    } else if (round) {
        result++;
    }
    return (int) result;
}

int division_fixed_point(int integer_part, int fractional_part, int element_1, int element_2) {
    int reverse = 0;
    if (check_negative(integer_part, fractional_part, element_1)) {
        element_1 = addition(integer_part, fractional_part, element_1);
        reverse++;
    }
    if (check_negative(integer_part, fractional_part, element_2)) {
        element_2 = addition(integer_part, fractional_part, element_2);
        reverse++;
    }
    long long result = (long long) element_1 << fractional_part;
    result /= (long long) element_2;
    long long round = ((long long) element_1 << fractional_part) % (long long) element_2;
    if (reverse % 2 == 1) {
        result = addition(integer_part, fractional_part, (int) result);
    } else if (round) {
        result++;
    }
    return (int) result;
}

pair<int, int> integer_and_fractional_parts(char *string) {
    char *dot;
    int integer_part = strtol(string, &dot, 10);
    dot++;
    int fractional_part = strtol(dot, &dot, 10);
    return {integer_part, fractional_part};
}

int main(int argc, char **argv) {
    if (argc == 4) {
        string rounding_type = argv[2];
        if (rounding_type == "2") {
            string data_type = argv[1];
            if (data_type == "f") {
                if (argv[3][0] == '0' && argv[3][1] == 'x') {
                    int element = int(strtoll(argv[3], nullptr, 16));
                    print_float(element);
                } else {
                    error_number();
                    return 1;
                }
            } else if (data_type == "h") {
                if (argv[3][0] == '0' && argv[3][1] == 'x') {
                    int element = int(strtol(argv[3], nullptr, 16));
                    print_half(element);
                } else {
                    error_number();
                    return 1;
                }
            } else {
                auto fixed_type = integer_and_fractional_parts(argv[1]);
                int integer_part = fixed_type.first;
                int fractional_part = fixed_type.second;
                int element = int(strtoll(argv[3], nullptr, 16));
                print_fixed_point(integer_part, fractional_part, element);
            }
        } else {
            error_message();
            return 1;
        }
    } else if (argc == 6) {
        string rounding_type = argv[2];
        if (rounding_type == "2") {
            string data_type = argv[1];
            if (data_type == "f") {
                string operation = argv[4];
                if (operation == "+") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = float_sum(element_1, element_2);
                        print_float(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else if (operation == "-") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = float_difference(element_1, element_2);
                        print_float(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else if (operation == "*") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = float_multiplication(element_1, element_2);
                        print_float(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else if (operation == "/") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = float_division(element_1, element_2);
                        print_float(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else {
                    error_operation();
                    return 1;
                }
            } else if (data_type == "h") {
                string operation = argv[4];
                if (operation == "+") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = half_sum(element_1, element_2);
                        print_half(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else if (operation == "-") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = half_difference(element_1, element_2);
                        print_half(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else if (operation == "*") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = half_multiplication(element_1, element_2);
                        print_half(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else if (operation == "/") {
                    if (argv[3][0] == '0' && argv[3][1] == 'x' && argv[5][0] == '0' && argv[5][1] == 'x') {
                        int element_1 = int(strtoll(argv[3], nullptr, 16));
                        int element_2 = int(strtoll(argv[5], nullptr, 16));
                        int result = half_division(element_1, element_2);
                        print_half(result);
                    } else {
                        error_number();
                        return 1;
                    }
                } else {
                    error_operation();
                    return 1;
                }
            } else {
                string operation = argv[4];
                if (operation == "+") {
                    auto fixed_type = integer_and_fractional_parts(argv[1]);
                    int integer_part = fixed_type.first;
                    int fractional_part = fixed_type.second;
                    int element_1 = int(strtoll(argv[3], nullptr, 16));
                    int element_2 = int(strtoll(argv[5], nullptr, 16));
                    int result = sum_fixed_point(element_1, element_2);
                    print_fixed_point(integer_part, fractional_part, result);
                } else if (operation == "-") {
                    auto fixed_type = integer_and_fractional_parts(argv[1]);
                    int integer_part = fixed_type.first;
                    int fractional_part = fixed_type.second;
                    int element_1 = int(strtoll(argv[3], nullptr, 16));
                    int element_2 = int(strtoll(argv[5], nullptr, 16));
                    int result = difference_fixed_point(integer_part, fractional_part, element_1, element_2);
                    print_fixed_point(integer_part, fractional_part, result);
                } else if (operation == "*") {
                    auto fixed_type = integer_and_fractional_parts(argv[1]);
                    int integer_part = fixed_type.first;
                    int fractional_part = fixed_type.second;
                    int element_1 = int(strtoll(argv[3], nullptr, 16));
                    int element_2 = int(strtoll(argv[5], nullptr, 16));
                    int result = multiplication_fixed_point(integer_part, fractional_part, element_1, element_2);
                    print_fixed_point(integer_part, fractional_part, result);
                } else if (operation == "/") {
                    auto fixed_type = integer_and_fractional_parts(argv[1]);
                    int integer_part = fixed_type.first;
                    int fractional_part = fixed_type.second;
                    int element_1 = int(strtoll(argv[3], nullptr, 16));
                    int element_2 = int(strtoll(argv[5], nullptr, 16));
                    if (element_2) {
                        int result = division_fixed_point(integer_part, fractional_part, element_1, element_2);
                        print_fixed_point(integer_part, fractional_part, result);
                    } else {
                        cout << "error\n";
                    }
                } else {
                    error_operation();
                    return 1;
                }
            }
        } else {
            error_message();
            return 1;
        }
    } else {
        error_message();
        return 1;
    }
}
