import 'dart:async';

import 'package:dio/dio.dart';
import 'package:flutter/material.dart';
import 'package:meshw/network/iot.dart';
import 'package:flutter_svg/svg.dart';
import 'package:meshw/persistent_storage/user_credential.dart';
import 'package:meshw/persistent_storage/user_token.dart';

class SensorsPage extends StatefulWidget {
  final UserCredential credential;
  final UserToken token;
  final Function() logOut;

  const SensorsPage({
    super.key,
    required this.credential,
    required this.token,
    required this.logOut,
  });

  @override
  State<SensorsPage> createState() => _SensorsPageState();
}

class _SensorsPageState extends State<SensorsPage> {
  double temperature = 0;
  double humidity = 0;
  double smoke = 0;
  bool showTemp = true;
  Timer? requestTimer;

  void requestSensorsData() async {
    try {
      Response response = await iot.getShadowData(
        userName: widget.credential.userName,
        password: widget.credential.password,
        token: widget.token.token,
      );
      _parseData(response);
    } on DioException catch (e) {
      String message = "";
      if (mounted) {
        switch (e.response?.statusCode) {
          case 400:
            message = "HTTP Error(400): Bad Request";
            break;
          case 401:
            message = "HTTP Error(401): Unauthorized";
            break;
          case 403:
            message = "HTTP Error(403): Token 错误";
            break;
          case 404:
            message = "HTTP Error(404): 未找到资源";
            break;
          case 500:
            message = "HTTP Error(500): 内部服务错误";
            break;
          default:
            message = e.message!;
        }
        debugPrint(message);
      }
    }
  }

  void _parseData(Response response) {
    temperature =
        response.data['shadow'][0]['reported']['properties']['temperature'];
    humidity = response.data['shadow'][0]['reported']['properties']['humidity'];
    smoke = response.data['shadow'][0]['reported']['properties']['smoke'];
    setState(() {});
    debugPrint('$temperature $humidity $smoke');
  }

  Timer getTimer() {
    return Timer.periodic(const Duration(seconds: 10), (timer) {
      requestSensorsData();
    });
  }

  @override
  void initState() {
    super.initState();
    requestSensorsData();
    requestTimer = getTimer();
  }

  @override
  void dispose() {
    requestTimer?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Container(
        constraints: const BoxConstraints(maxWidth: 500),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            children: [
              Card(
                clipBehavior: Clip.antiAlias,
                child: InkWell(
                  onTap: () {
                    setState(() {
                      showTemp = !showTemp;
                    });
                  },
                  child: Padding(
                    padding: const EdgeInsets.all(16),
                    child: Row(
                      children: [
                        SvgPicture.asset(
                          showTemp
                              ? 'assets/images/temperature.svg'
                              : 'assets/images/humidity.svg',
                          width: 64,
                          colorFilter: ColorFilter.mode(
                            showTemp ? Colors.orangeAccent : Colors.blueAccent,
                            BlendMode.srcIn,
                          ),
                        ),
                        const SizedBox(width: 8),
                        Text(
                          showTemp ? '温度: $temperature °C' : '湿度: $humidity %',
                          style: const TextStyle(fontSize: 20),
                        ),
                      ],
                    ),
                  ),
                ),
              ),
              const SizedBox(height: 8),
              Card(
                child: Padding(
                  padding: const EdgeInsets.all(16),
                  child: Row(
                    children: [
                      SvgPicture.asset(
                        'assets/images/smoke.svg',
                        width: 64,
                        colorFilter: const ColorFilter.mode(
                          Colors.blueGrey,
                          BlendMode.srcIn,
                        ),
                      ),
                      const SizedBox(width: 8),
                      Text(
                        '烟雾浓度: $smoke %',
                        style: const TextStyle(fontSize: 20),
                      ),
                    ],
                  ),
                ),
              ),
              const SizedBox(height: 8),
              FilledButton.tonal(
                onPressed: widget.logOut,
                style: ButtonStyle(
                  backgroundColor: WidgetStateProperty.resolveWith((_) {
                    return Theme.of(context).colorScheme.primary.withAlpha(20);
                  }),
                ),
                child: Text(
                  "退出登录",
                  style:
                      TextStyle(color: Theme.of(context).colorScheme.primary),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
