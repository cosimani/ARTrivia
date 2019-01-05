import QtQuick 2.0
import QtMultimedia 5.4

Rectangle  {
    id : cameraUI

//    color: "#A90230"
    color: "#ACB353"

    signal signal_imageProcessing(string msg)
    signal signal_imagePath(string msg)

//    Timer  {
//        id: timer
//        interval: 5000; running: false; repeat: false
//        onTriggered: {
////            timerEfecto.start()
//        }
//    }

    Camera {
        id: camera

        flash.mode: Camera.FlashAuto

//        position: Camera.FrontFace
        position: Camera.BackFace

//        captureMode: Camera.CaptureStillImage
//        captureMode: Camera.CaptureViewfinder

        // Ver este ejemplo: http://stackoverflow.com/questions/37800217/recording-video-to-file-with-qml
        captureMode: Camera.CaptureVideo

//        videoRecorder.mediaContainer: "mp4"


        imageCapture  {
            //            resolution: "640x480"
            onImageCaptured: {

                cameraUI.signal_imageProcessing(preview)

//                viewfinder.layer.enabled = true
//                timerEfecto.start()

//                console.log("cameraUI", cameraUI.width, cameraUI.height,
//                            "parent", parent.width, parent.height,
//                            "viewfinder", viewfinder.width, viewfinder.height);

            }

            onImageSaved: {
                cameraUI.signal_imagePath(path)
            }

        }
    }


    MouseArea {
        anchors.fill: parent
        onClicked: camera.imageCapture.capture()
    }

    VideoOutput {
        id: viewfinder

        anchors.fill: parent

        source: camera
        autoOrientation: true

//        MouseArea  {
//            anchors.fill: parent;
//            onClicked:  {
//                camera.videoRecorder.record();
//            }
//        }

//        layer.enabled: true
        layer.enabled: false

            layer.effect:
                ShaderEffect  {

                property real iGlobalTime: 0
                property real iTimeDeltaSpeed: 1

                Timer
                    {
                        id: timerEfecto
                        running: true
                        repeat: true
                        interval: 6000
//                        interval: 1000
                        onTriggered:
                        {
                            iGlobalTime += iTimeDeltaSpeed;

                            if ( iGlobalTime >= 20 )  {
                                iGlobalTime = 0;
                            }
                        }
                    }

                    fragmentShader: "

                        uniform lowp float iGlobalTime;

                        uniform lowp sampler2D source; // this item
                        uniform lowp float qt_Opacity; // inherited opacity of this item
                        varying highp vec2 qt_TexCoord0;

                        void main()  {

                            if ( iGlobalTime >= 0. && iGlobalTime < 10. )  {  // Ningun efecto
                                gl_FragColor = texture2D(source, qt_TexCoord0);
                            }
                            else if ( iGlobalTime >= 10. && iGlobalTime < 11. )  {  // Las burbujas
                                float grid = 12.0;

                                vec2 uv = qt_TexCoord0.xy;
                                float offx = floor(uv.x  / (grid * 0.0015625));
                                float offy = floor(uv.y  / (grid * 0.0015625));
                                vec3 res = texture2D(source, vec2(offx * grid * 0.0015625 , offy * grid * 0.0015625)).rgb;
                                vec2 prc = fract(uv / vec2(grid * 0.0015625, grid * 0.0015625));

                                vec2 pw = pow(abs(prc - 0.5), vec2(2.0));
                                float  rs = pow(0.45, 2.0);
                                float gr = smoothstep(rs - 0.1, rs + 0.1, pw.x + pw.y);
                                float y = (res.r + res.g + res.b) / 3.0;
                                vec3 ra = res / y;
                                float ls = 0.3;
                                float lb = ceil(y / ls);
                                float lf = ls * lb + 0.3;
                                res = lf * res;
                                vec3 col = mix(res, vec3(0.1, 0.1, 0.1), gr);

                                gl_FragColor = qt_Opacity * vec4(col, 1.0);
                            }
                            else if ( iGlobalTime >= 11. && iGlobalTime < 12. )  {  // Blanco y negro
                                lowp vec4 p = texture2D(source, qt_TexCoord0);
                                lowp float g = dot(p.xyz, vec3(0.344, 0.5, 0.156));
                                gl_FragColor = vec4(g, g, g, p.a) * qt_Opacity;
                            }
                            else if ( iGlobalTime >= 12. && iGlobalTime < 13. )  {  // Olas verticales
                                highp vec2 wiggledTexCoord = qt_TexCoord0;
                                wiggledTexCoord.s += sin(4.0 * 3.141592653589 * wiggledTexCoord.t) * 0.25;
                                gl_FragColor = texture2D(source, wiggledTexCoord.st);
                            }
                            else if ( iGlobalTime >= 13. && iGlobalTime < 14. )  {
                                vec2 uv = qt_TexCoord0.xy;
                                vec4 c = vec4(0.0);
                                c += texture2D(source, uv - vec2(4.0*0.01, 0.0)) * 0.05;
                                c += texture2D(source, uv - vec2(3.0*0.01, 0.0)) * 0.09;
                                c += texture2D(source, uv - vec2(2.0*0.01, 0.0)) * 0.12;
                                c += texture2D(source, uv - vec2(1.0*0.01, 0.0)) * 0.15;
                                c += texture2D(source, uv) * 0.18;
                                c += texture2D(source, uv + vec2(1.0*0.01, 0.0)) * 0.15;
                                c += texture2D(source, uv + vec2(2.0*0.01, 0.0)) * 0.12;
                                c += texture2D(source, uv + vec2(3.0*0.01, 0.0)) * 0.09;
                                c += texture2D(source, uv + vec2(4.0*0.01, 0.0)) * 0.05;
                                gl_FragColor = qt_Opacity * c;
                            }
                            else if ( iGlobalTime >= 14. && iGlobalTime < 15. )  {
                                vec2 uv = qt_TexCoord0.xy;
                                vec2 tc = qt_TexCoord0;
                                float dx = 5. / 320.;
                                float dy = 5. / 240.;
                                tc = vec2(dx*(floor(uv.x/dx) + 0.5), dy*(floor(uv.y/dy) + 0.5));
                                gl_FragColor = qt_Opacity * texture2D(source, tc);
                            }
                            else if ( iGlobalTime >= 15. && iGlobalTime < 16. )  {
                                vec2 uv = qt_TexCoord0.xy;
                                vec4 c = vec4(0.0);
                                vec3 x = texture2D(source, uv).rgb;
                                x = pow(x, vec3(5., 5., 5.));
                                x = x * 16.;
                                x = floor(x);
                                x = x / 16.;
                                x = pow(x, vec3(1.0/5.));
                                c = vec4(x, 1.0);
                                gl_FragColor = qt_Opacity * c;
                            }
                            else if ( iGlobalTime >= 16. && iGlobalTime < 17. )  {
                                    vec2 uv = qt_TexCoord0.xy;
                                    vec2 tc = qt_TexCoord0;
                                    if (0.5 * 20. > 0.0) {
                                        float dx = 0.5 * 20. / 320.;
                                        float dy = 0.5 * 20. / 240.;
                                        tc = vec2(dx*(floor(uv.x/dx) + 0.5),
                                                  dy*(floor(uv.y/dy) + 0.5));
                                    }
                                    gl_FragColor = qt_Opacity * texture2D(source, tc);
                            }
                            else if ( iGlobalTime >= 17. && iGlobalTime < 18. )  {
                                vec2 uv = qt_TexCoord0.xy;
                                vec4 orig = texture2D(source, uv);
                                vec3 col = orig.rgb;
                                float y = 0.3 * col.r + 0.59 * col.g + 0.11 * col.b;
                                gl_FragColor = qt_Opacity * vec4(y + 0.15, y + 0.07, y - 0.12, 1.0);
                            }
                            else if ( iGlobalTime >= 18. && iGlobalTime < 19. )  {
                                vec2 uv = qt_TexCoord0.xy;
                                vec4 orig = texture2D(source, uv);
                                float cr = pow(0.1, 2.0);
                                float pt = pow(uv.x - 0.5, 2.0) + pow(uv.y - 0.5, 2.0);
                                float d = pt - cr;
                                float cf = 1.0;
                                if (d > 0.0)
                                    cf = 1.0 - 2.0 * d;
                                vec3 col = cf * orig.rgb;
                                gl_FragColor = qt_Opacity * vec4(col, 1.0);
                            }
                            else if ( iGlobalTime >= 19. && iGlobalTime < 20. )  {
                                vec2 uv = qt_TexCoord0.xy;
                                vec4 orig = texture2D(source, uv);
                                vec3 col = orig.rgb;
                                float y = 0.3 *col.r + 0.59 * col.g + 0.11 * col.b;
                                y = y < 0.3 ? 0.0 : (y < 0.6 ? 0.5 : 1.0);
                                if (y == 0.5)
                                    col = vec3(0.8, 0.0, 0.0);
                                else if (y == 1.0)
                                    col = vec3(0.9, 0.9, 0.0);
                                else
                                    col = vec3(0.0, 0.0, 0.0);
                                gl_FragColor = qt_Opacity * vec4(col, 1.0);
                            }
                            else  {
                                gl_FragColor = texture2D(source, qt_TexCoord0);
                            }
                        }"
                  }
    }
}
