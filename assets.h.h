#ifndef ASSETS_H
#define ASSETS_H

/* Asset Management module*/

void   assetMenu(void);            /* sub-menu,and it's called from main.c            */
void   addAsset(void);             /* this adds one more asset to the register           */
void   displayAssets(void);        /* this display every registered asset          */
void   searchAsset(void);          /* search the ID, type or department        */
void   assetReport(void);          /* asset report, called from reports.c     */
int    getAssetCount(void);        /* number of registered assets             */
double getTotalAssetValue(void);   /* sum of all purchase values              */

#endif
