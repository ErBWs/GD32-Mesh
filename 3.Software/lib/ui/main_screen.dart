import 'package:flutter/material.dart';
import 'package:meshw/network/iot.dart';

class MainScreen extends StatefulWidget {
  const MainScreen({super.key});

  @override
  State<MainScreen> createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> {
  final formKey = GlobalKey<FormState>();
  final projectIDController = TextEditingController();
  final deviceIDController = TextEditingController();
  final userNameController = TextEditingController();
  final passwordController = TextEditingController();

  bool passwordVisible = false;
  String a = "";

  Widget _buildLoginForm() {
    return Form(
      key: formKey,
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            TextFormField(
              controller: projectIDController,
              decoration: InputDecoration(
                labelText: '项目ID',
                suffixIcon: IconButton(
                  onPressed: () {
                    projectIDController.clear();
                  },
                  icon: const Icon(Icons.cancel_outlined),
                ),
                filled: true,
              ),
              validator: (value) => value?.isEmpty ?? true ? '请输入项目ID' : null,
            ),
            const SizedBox(height: 8),
            TextFormField(
              controller: deviceIDController,
              decoration: InputDecoration(
                labelText: '设备ID',
                suffixIcon: IconButton(
                  onPressed: () {
                    deviceIDController.clear();
                  },
                  icon: const Icon(Icons.cancel_outlined),
                ),
                filled: true,
              ),
              validator: (value) => value?.isEmpty ?? true ? '请输入设备ID' : null,
            ),
            const SizedBox(height: 8),
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
                await iot.getAuthToken(projectID: 'projectID', userName: 'userName', password: passwordController.text);
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
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.primaryContainer,
        title: const Text("Mesh-Watcher"),
      ),
      body: _buildLoginForm(),// This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
