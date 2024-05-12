#define float float
#define FIS_RESOLUSION 101
#define FIS_MIN -3.4028235E+38
#define FIS_MAX 3.4028235E+38
typedef float(*_FIS_MF)(float, float*);
typedef float(*_FIS_ARR_OP)(float, float);
typedef float(*_FIS_ARR)(float*, int, _FIS_ARR_OP);

const int num_inputs = 2;
const int num_outputs = 1;
const int num_rules = 35;

float fis_input[num_inputs];
float fis_output[num_outputs];

//ranges
float input_range_min[] = { 0, -1 };
float input_range_max[] = { 100, 1 };
float output_range_min[] = { 0 };
float output_range_max[] = { 1 };

void setup()
{
    Serial.begin(115200);

}
void loop()
{
    // Read Input: Speed 0-100
    fis_input[0] =40 ;
    // Read Input: Pothole_Bump -1 to 1
    fis_input[1] = -0.5;

   fis_output[0] = 0;

    fis_evaluate();
    Serial.println(fis_output[0]);
    

}

//Func Definations
float triangularMF(float x, float* p)
{
    float a = p[0], b = p[1], c = p[2];
    float t1 = (x - a) / (b - a);
    float t2 = (c - x) / (c - b);
    if ((a == b) && (b == c)) return (float) (x == a);
    if (a == b) return (float) (t2*(b <= x)*(x <= c));
    if (b == c) return (float) (t1*(a <= x)*(x <= b));
    t1 = min(t1, t2);
    return (float) max(t1, 0);
}

float fis_min(float a, float b)
{
    return min(a, b);
}

float fis_max(float a, float b)
{
    return max(a, b);
}

float fis_prod(float a, float b)
{
    return (a * b);
}

float fis_sum(float a, float b)
{
    return (a + b);
}

float fis_array_operation(float *array, int size, _FIS_ARR_OP pfnOp)
{
    int i;
    float ret = 0;

    if (size == 0) return ret;
    if (size == 1) return array[0];

    ret = array[0];
    for (i = 1; i < size; i++)
    {
        ret = (*pfnOp)(ret, array[i]);
    }

    return ret;
}

//Data
_FIS_MF triMF_[] =
{
    triangularMF
};

int input_MF_count[] = { 5, 7 };
int output_MF_count[] = { 5 };
//data i/p
float triMF_coeff_speed_mf1[] = { 0, 0, 20.8333333333333 };
float triMF_coeff_speed_mf2[] = { 4.16666666666667, 25, 45.8333333333333 };
float triMF_coeff_speed_mf3[] = { 29.1666666666667, 50, 70.8333333333333 };
float triMF_coeff_speed_mf4[] = { 54.1666666666667, 75, 95.8333333333333 };
float triMF_coeff_speed_mf5[] = { 79.1666666666667, 100, 120.833333333333 };
float* triMF_coeff_speed_mf[] = { triMF_coeff_speed_mf1, triMF_coeff_speed_mf2, triMF_coeff_speed_mf3, triMF_coeff_speed_mf4, triMF_coeff_speed_mf5 };
float triMF_coeff_bump_mf1[] = { -1.27777777777778, -1, -0.722222222222222 };
float triMF_coeff_bump_mf2[] = { -0.944444444444444, -0.666666666666667, -0.388888888888889 };
float triMF_coeff_bump_mf3[] = { -0.611111111111111, -0.333333333333333, -0.0555555555555557 };
float triMF_coeff_bump_mf4[] = { -0.277777777777778, -1.11022302462516e-16, 0.277777777777778 };
float triMF_coeff_bump_mf5[] = { 0.0555555555555555, 0.333333333333333, 0.611111111111111 };
float triMF_coeff_bump_mf6[] = { 0.388888888888889, 0.666666666666667, 0.944444444444444 };
float triMF_coeff_bump_mf7[] = { 0.722222222222222, 1, 1.27777777777778 };
float* triMF_coeff_bump_mf[] = { triMF_coeff_bump_mf1, triMF_coeff_bump_mf2, triMF_coeff_bump_mf3, triMF_coeff_bump_mf4, triMF_coeff_bump_mf5, triMF_coeff_bump_mf6, triMF_coeff_bump_mf7 };
float** triMF_ICoeff[] = { triMF_coeff_speed_mf, triMF_coeff_bump_mf };

//data o/p
float triMF_coeff_stiffness_mf1[] = { 0, 0, 0 };
float triMF_coeff_stiffness_mf2[] = { 0, 0, 0.25 };
float triMF_coeff_stiffness_mf3[] = { 0, 0, 0.5 };
float triMF_coeff_stiffness_mf4[] = { 0, 0, 0.75 };
float triMF_coeff_stiffness_mf5[] = { 0, 0, 1 };
float* triMF_coeff_stiffness_mf[] = { triMF_coeff_stiffness_mf1, triMF_coeff_stiffness_mf2, triMF_coeff_stiffness_mf3, triMF_coeff_stiffness_mf4, triMF_coeff_stiffness_mf5 };
float** triMF_OCoeff[] = { triMF_coeff_stiffness_mf };

// Input membership function set
int triMF_I0[] = { 0, 0, 0, 0, 0 };
int triMF_I1[] = { 0, 0, 0, 0, 0, 0, 0 };
int* triMF_I[] = { triMF_I0, triMF_I1};

// Output membership function set
int* triMF_O[] = {};

//rules
float rule_weights[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int rule_types[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
//rule i/p
int rule_num0[] = { 1, 1 };
int rule_num1[] = { 2, 1 };
int rule_num2[] = { 3, 1 };
int rule_num3[] = { 4, 1 };
int rule_num4[] = { 5, 1 };
int rule_num5[] = { 1, 2 };
int rule_num6[] = { 2, 2 };
int rule_num7[] = { 3, 2 };
int rule_num8[] = { 4, 2 };
int rule_num9[] = { 5, 2 };
int rule_num10[] = { 1, 3 };
int rule_num11[] = { 2, 3 };
int rule_num12[] = { 3, 3 };
int rule_num13[] = { 4, 3 };
int rule_num14[] = { 5, 3 };
int rule_num15[] = { 1, 4 };
int rule_num16[] = { 2, 4 };
int rule_num17[] = { 3, 4 };
int rule_num18[] = { 4, 4 };
int rule_num19[] = { 5, 4 };
int rule_num20[] = { 1, 5 };
int rule_num21[] = { 2, 5 };
int rule_num22[] = { 3, 5 };
int rule_num23[] = { 4, 5 };
int rule_num24[] = { 5, 5 };
int rule_num25[] = { 1, 6 };
int rule_num26[] = { 2, 6 };
int rule_num27[] = { 3, 6 };
int rule_num28[] = { 4, 6 };
int rule_num29[] = { 5, 6 };
int rule_num30[] = { 1, 7 };
int rule_num31[] = { 2, 7 };
int rule_num32[] = { 3, 7 };
int rule_num33[] = { 4, 7 };
int rule_num34[] = { 5, 7 };
int* rule_num[] = { rule_num0, rule_num1, rule_num2, rule_num3, rule_num4, rule_num5, rule_num6, rule_num7, rule_num8, rule_num9, rule_num10, rule_num11, rule_num12, rule_num13, rule_num14, rule_num15, rule_num16, rule_num17, rule_num18, rule_num19, rule_num20, rule_num21, rule_num22, rule_num23, rule_num24, rule_num25, rule_num26, rule_num27, rule_num28, rule_num29, rule_num30, rule_num31, rule_num32, rule_num33, rule_num34 };

// rule o/p
int rule_output0[] = { 1 };
int rule_output1[] = { 1 };
int rule_output2[] = { 2 };
int rule_output3[] = { 3 };
int rule_output4[] = { 3 };
int rule_output5[] = { 1 };
int rule_output6[] = { 2 };
int rule_output7[] = { 3 };
int rule_output8[] = { 3 };
int rule_output9[] = { 3 };
int rule_output10[] = { 2 };
int rule_output11[] = { 2 };
int rule_output12[] = { 3 };
int rule_output13[] = { 3 };
int rule_output14[] = { 4 };
int rule_output15[] = { 4 };
int rule_output16[] = { 4 };
int rule_output17[] = { 4 };
int rule_output18[] = { 5 };
int rule_output19[] = { 5 };
int rule_output20[] = { 1 };
int rule_output21[] = { 1 };
int rule_output22[] = { 2 };
int rule_output23[] = { 3 };
int rule_output24[] = { 3 };
int rule_output25[] = { 2 };
int rule_output26[] = { 3 };
int rule_output27[] = { 3 };
int rule_output28[] = { 4 };
int rule_output29[] = { 4 };
int rule_output30[] = { 3 };
int rule_output31[] = { 4 };
int rule_output32[] = { 4 };
int rule_output33[] = { 5 };
int rule_output34[] = { 5 };
int* rule_output[] = { rule_output0, rule_output1, rule_output2, rule_output3, rule_output4, rule_output5, rule_output6, rule_output7, rule_output8, rule_output9, rule_output10, rule_output11, rule_output12, rule_output13, rule_output14, rule_output15, rule_output16, rule_output17, rule_output18, rule_output19, rule_output20, rule_output21, rule_output22, rule_output23, rule_output24, rule_output25, rule_output26, rule_output27, rule_output28, rule_output29, rule_output30, rule_output31, rule_output32, rule_output33, rule_output34 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System           
//***********************************************************************
// None for Sugeno

//***********************************************************************
// Fuzzy Inference System                                                
//***********************************************************************
void fis_evaluate()
{
    float fuzzyInput0[] = { 0, 0, 0, 0, 0 };
    float fuzzyInput1[] = { 0, 0, 0, 0, 0, 0, 0 };
    float* fuzzyInput[num_inputs] = { fuzzyInput0, fuzzyInput1, };
    float fuzzyOutput0[] = { 0, 0, 0, 0, 0 };
    float* fuzzyOutput[num_outputs] = { fuzzyOutput0, };
    float fuzzyRules[num_rules] = { 0 };
    float fuzzyFires[num_rules] = { 0 };
    float* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
    float sW = 0;

    // Transforming input to fuzzy Input
    int i, j, r, o;
    for (i = 0; i < num_inputs; ++i)
    {
        for (j = 0; j < input_MF_count[i]; ++j)
        {
            fuzzyInput[i][j] =
                (triMF_[triMF_I[i][j]])(fis_input[i], triMF_ICoeff[i][j]);
        }
    }

    int index = 0;
    for (r = 0; r < num_rules; ++r)
    {
        if (rule_types[r] == 1)
        {
            fuzzyFires[r] = FIS_MAX;
            for (i = 0; i < num_inputs; ++i)
            {
                index = rule_num[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_min(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_min(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_min(fuzzyFires[r], 1);
            }
        }
        else
        {
            fuzzyFires[r] = FIS_MIN;
            for (i = 0; i < num_inputs; ++i)
            {
                index = rule_num[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_max(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_max(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_max(fuzzyFires[r], 0);
            }
        }

        fuzzyFires[r] = rule_weights[r] * fuzzyFires[r];
        sW += fuzzyFires[r];
    }

    if (sW == 0)
    {
        for (o = 0; o < num_outputs; ++o)
        {
           fis_output[o] = ((output_range_max[o] + output_range_min[o]) / 2);
        }
    }
    else
    {
        for (o = 0; o < num_outputs; ++o)
        {
            float sWI = 0.0;
            for (j = 0; j < output_MF_count[o]; ++j)
            {
                fuzzyOutput[o][j] = triMF_OCoeff[o][j][num_inputs];
                for (i = 0; i < num_inputs; ++i)
                {
                    fuzzyOutput[o][j] += fis_input[i] * triMF_OCoeff[o][j][i];
                }
            }

            for (r = 0; r < num_rules; ++r)
            {
                index = rule_output[r][o] - 1;
                sWI += fuzzyFires[r] * fuzzyOutput[o][index];
            }

           fis_output[o] = sWI / sW;
        }
    }
}
