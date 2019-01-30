package net.searock.apikeysinndk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("keys");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tvKey = findViewById(R.id.tvKey);
        tvKey.setText(getApiKey());

        Encryption encryption = new Encryption();
        String apiKey = "MyApiKey";

        Cert cert = new Cert();
        int signatureHashCode = cert.getCertificateHash(this);

        try{

            String encryptedData = encryption.encryptData(Integer.toString(signatureHashCode),
                    apiKey);
            Log.e("encryptedData", encryptedData);

            String decryptedData = encryption.decryptData(Integer.toString(signatureHashCode),
                    encryptedData);
            Log.e("decryptedData", decryptedData);

        }catch (Exception e){

        }

    }

    private native String getApiKey();
}
