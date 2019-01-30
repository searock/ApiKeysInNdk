package net.searock.apikeysinndk;

import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

public class Cert {

    public int getCertificateHash(Context context){
        try{
            Signature sig = context.getPackageManager().getPackageInfo(context.getPackageName(),
                    PackageManager.GET_SIGNATURES).signatures[0];

            int hashCode = sig.hashCode();

            return hashCode;
        }catch (Exception e){

        }

        return 0;
    }
}
