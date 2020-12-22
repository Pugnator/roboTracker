#include "FIRQ31.h"
#include <stdio.h>
#include "Error.h"

#define SNR_THRESHOLD 100

#define ABS_ERROR_Q31 ((q31_t)2)

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
static __ALIGNED(8) q31_t coeffArray[32];
#endif

static void checkInnerTail(q31_t *b)
{
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 0);
    ASSERT_TRUE(b[2] == 0);
    ASSERT_TRUE(b[3] == 0);
}


    void FIRQ31::test_fir_q31()
    {
        

        const int16_t *configp = configs.ptr();
        q31_t *statep = state.ptr();
        const q31_t *orgcoefsp = coefs.ptr();

        const q31_t *coefsp;
        const q31_t *inputp = inputs.ptr();
        q31_t *outp = output.ptr();

        unsigned long i;
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
        int j;
#endif
        int blockSize;
        int numTaps;
        int nb=1;

        /*

        Python script is generating different tests with
        different blockSize and numTaps.

        We loop on those configs.

        */
        for(i=0; i < configs.nbSamples() ; i += 2)
        {
           blockSize = configp[0];
           numTaps = configp[1];

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
           /* Copy coefficients and pad to zero 
           */
           memset(coeffArray,0,32*sizeof(q31_t));
           for(j=0;j < numTaps; j++)
           {
              coeffArray[j] = orgcoefsp[j];
           }
   
           coefsp = coeffArray;
#else
           coefsp = orgcoefsp;
#endif

           /*

           The filter is initialized with the coefs, blockSize and numTaps.

           */
           arm_fir_init_q31(&this->S,numTaps,coefsp,statep,blockSize);

           /*

           Input pointer is reset since the same input pattern is used

           */
           inputp = inputs.ptr();

           /*
           
           Python script is filtering a 2*blockSize number of samples.
           We do the same filtering in two pass to check (indirectly that
           the state management of the fir is working.)

           */
           arm_fir_q31(&this->S,inputp,outp,blockSize);
           outp += blockSize;
           checkInnerTail(outp);

           inputp += blockSize;
           arm_fir_q31(&this->S,inputp,outp,blockSize);
           outp += blockSize;
           checkInnerTail(outp);

           configp += 2;
           orgcoefsp += numTaps;

           nb += blockSize + blockSize;


        }

        ASSERT_EMPTY_TAIL(output);

        ASSERT_SNR(output,ref,(q31_t)SNR_THRESHOLD);

        ASSERT_NEAR_EQ(output,ref,ABS_ERROR_Q31);

    } 

 
    void FIRQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {
      
       (void)params;
       switch(id)
       {
        case FIRQ31::TEST_FIR_Q31_1:
          
        break;

       }
      

       inputs.reload(FIRQ31::FIRINPUTS_Q31_ID,mgr);
       coefs.reload(FIRQ31::FIRCOEFS_Q31_ID,mgr);
       configs.reload(FIRQ31::FIRCONFIGS_S16_ID,mgr);
       ref.reload(FIRQ31::FIRREFS_Q31_ID,mgr);

       output.create(ref.nbSamples(),FIRQ31::OUT_Q31_ID,mgr);
       /* > Max blockSize + numTaps - 1 as generated by Python script */
       state.create(47 + 47+47,FIRQ31::OUT_Q31_ID,mgr);
    }

    void FIRQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        output.dump(mgr);
    }