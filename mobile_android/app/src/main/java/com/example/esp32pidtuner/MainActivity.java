package com.example.esp32pidtuner;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {

    private EditText kpInput, kiInput, kdInput, ipInput;
    private Button sendButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        kpInput = findViewById(R.id.kp_input);
        kiInput = findViewById(R.id.ki_input);
        kdInput = findViewById(R.id.kd_input);
        ipInput = findViewById(R.id.ip_input);
        sendButton = findViewById(R.id.send_button);

        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String kp = kpInput.getText().toString().trim();
                String ki = kiInput.getText().toString().trim();
                String kd = kdInput.getText().toString().trim();
                String ip = ipInput.getText().toString().trim();

                if (kp.isEmpty() || ki.isEmpty() || kd.isEmpty() || ip.isEmpty()) {
                    Toast.makeText(MainActivity.this, "Please fill all fields", Toast.LENGTH_SHORT).show();
                    return;
                }

                sendPID(ip, kp, ki, kd);
            }
        });
    }

    private void sendPID(final String ip, final String kp, final String ki, final String kd) {
        new Thread(() -> {
            try {
                String urlString;

                // ✅ If user entered a full URL, use it directly (even if it's http://webhook.site/...)
                if (ip.startsWith("http://") || ip.startsWith("https://")) {
                    urlString = ip;
                } else {
                    // ✅ Otherwise assume it’s a local ESP32 IP and append /setPID
                    urlString = "http://" + ip + "/setPID";
                }

                runOnUiThread(() ->
                        Toast.makeText(MainActivity.this, "Sending to: " + urlString, Toast.LENGTH_SHORT).show()
                );

                URL url = new URL(urlString);
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestMethod("POST");
                conn.setDoOutput(true);
                conn.setRequestProperty("Content-Type", "application/json; utf-8");

                // ✅ JSON body
                String json = String.format("{\"kp\":%s,\"ki\":%s,\"kd\":%s}", kp, ki, kd);

                try (OutputStream os = conn.getOutputStream()) {
                    os.write(json.getBytes("utf-8"));
                }

                int responseCode = conn.getResponseCode();
                runOnUiThread(() ->
                        Toast.makeText(MainActivity.this, "Sent! Response: " + responseCode, Toast.LENGTH_SHORT).show());

                conn.disconnect();

            } catch (Exception e) {
                runOnUiThread(() ->
                        Toast.makeText(MainActivity.this, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show());
            }
        }).start();
    }
}