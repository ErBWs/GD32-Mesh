import 'package:dio/dio.dart';
import 'package:flutter/material.dart';
import 'package:meshw/network/iot.dart';
import 'package:meshw/persistent_storage/hive_instance.dart';
import 'package:meshw/persistent_storage/user_credential.dart';
import 'package:meshw/persistent_storage/user_token.dart';

class MainScreen extends StatefulWidget {
  const MainScreen({super.key});

  @override
  State<MainScreen> createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> {
  final formKey = GlobalKey<FormState>();
  final userNameController = TextEditingController();
  final passwordController = TextEditingController();

  late UserToken token;
  late UserCredential credential;

  bool passwordVisible = false;

  Widget _buildLoginForm() {
    return Form(
      key: formKey,
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            TextFormField(
              controller: userNameController,
              decoration: InputDecoration(
                labelText: '用户名',
                suffixIcon: IconButton(
                  onPressed: () {
                    userNameController.clear();
                  },
                  icon: const Icon(Icons.cancel_outlined),
                ),
                filled: true,
              ),
              validator: (value) => value?.isEmpty ?? true ? '请输入用户名' : null,
            ),
            const SizedBox(height: 8),
            TextFormField(
              controller: passwordController,
              keyboardType: TextInputType.visiblePassword,
              decoration: InputDecoration(
                labelText: '密码',
                suffixIcon: IconButton(
                  onPressed: () {
                    setState(() {
                      passwordVisible = !passwordVisible;
                    });
                  },
                  icon: Icon(passwordVisible
                      ? Icons.visibility_rounded
                      : Icons.visibility_off_rounded),
                ),
                filled: true,
              ),
              obscureText: !passwordVisible,
              validator: (value) => value?.isEmpty ?? true ? '请输入密码' : null,
            ),
            const SizedBox(height: 8),
            FilledButton.tonal(
              onPressed: () async {
                if (formKey.currentState!.validate()) {
                  try {
                    token = await iot.getAuthToken(
                      userName: userNameController.text,
                      password: passwordController.text,
                    );
                    credential.userName = userNameController.text;
                    credential.password = passwordController.text;
                    if (token.token.isNotEmpty) {
                      debugPrint(token.token);
                      hiveInstance.userToken.put('token', token);
                      hiveInstance.userCredential.put('credential', credential);
                    }
                  } on DioException catch (e) {
                    String message = "";
                    if (mounted) {
                      switch (e.response?.statusCode) {
                        case 401:
                          message = "HTTP Error(401): 登录失败，请检查用户名和密码";
                          break;
                        case 403:
                          message = "HTTP Error(403): 登录失败，该用户没有权限";
                          break;
                        case 404:
                          message = "HTTP Error(404): 未找到资源";
                          break;
                        case 500:
                          message = "HTTP Error(500): 内部服务错误";
                          break;
                        case 503:
                          message = "HTTP Error(503): 服务不可用";
                          break;
                        default:
                          message = e.message!;
                      }
                      ScaffoldMessenger.of(context)
                          .showSnackBar(SnackBar(content: Text(message)));
                    }
                  }
                }
              },
              style: ButtonStyle(
                backgroundColor: WidgetStateProperty.resolveWith((_) {
                  return Theme.of(context).colorScheme.primary.withAlpha(20);
                }),
              ),
              child: Text(
                "登录",
                style: TextStyle(color: Theme.of(context).colorScheme.primary),
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void initState() {
    super.initState();
    token = hiveInstance.userToken.get(
      'token',
      defaultValue: UserToken(
        token: "",
        time: DateTime.now().millisecondsSinceEpoch,
      ),
    )!;
    credential = hiveInstance.userCredential.get(
      'credential',
      defaultValue: UserCredential(
        userName: "",
        password: "",
      ),
    )!;
    userNameController.text = credential.userName;
    passwordController.text = credential.password;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.primaryContainer,
        title: const Text("Mesh-Watcher"),
      ),
      body: _buildLoginForm(),
    );
  }
}
