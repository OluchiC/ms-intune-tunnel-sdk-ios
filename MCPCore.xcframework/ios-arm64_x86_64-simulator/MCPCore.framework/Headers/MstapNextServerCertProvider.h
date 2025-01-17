//
//  MstapNextServerCertProvider.h
//  mobile_mstapnext_core
//
//  Created by Nehal Odedra on 7/1/16.
//  Licensed to Microsoft under Contract #7267038.
//

#ifndef MstapNextServerCertProvider_h
#define MstapNextServerCertProvider_h

#include <stdio.h>
#include "atlas_client_types.h"

#pragma GCC visibility push(default)

#ifdef __cplusplus
extern "C" {
#endif
__attribute__ ((visibility ("default")))
@interface MstapNextServerCertProvider : NSObject
+ (void)MAP_trustAugServerCerts:(const std::vector<std::string> &)certlist
          blockNetworkResources:(bool)blockNetworkResources;
+ (void)MAP_trustAugInitRelinks;
@end
CFArrayRef MAP_ConvertPEMCertsToCF(const std::vector<std::string> &certlist);
CFArrayRef MAP_ConvertDERCertsToCF(const std::vector<std::vector<uint8_t>> &certlist);
void MAP_AddTrustedCerts(SecTrustRef trust, CFArrayRef trustAugServerCerts);
OSStatus MAP_SecTrustEvaluate(SecTrustRef trust, SecTrustResultType *result);
bool MAP_SecTrustEvaluateWithError(SecTrustRef  _Nonnull trust, CFErrorRef  _Nullable * _Nullable error);
OSStatus MAP_SecTrustEvaluateAsync(SecTrustRef trust, dispatch_queue_t queue, SecTrustCallback result);
OSStatus MAP_SecTrustEvaluateAsyncWithError(SecTrustRef trust, dispatch_queue_t queue, SecTrustWithErrorCallback result);
#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop

#endif /* MstapNextServerCertProvider_h */
