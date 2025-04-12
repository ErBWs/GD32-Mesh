import 'package:flutter/material.dart';
import 'package:meshw/persistent_storage/hive_instance.dart';
import 'package:meshw/ui/login_page.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  try {
    await hiveInstance.init();
  } catch (_) {
    runApp(MaterialApp(
      title: '初始化失败',
      builder: (context, child) {
        return Scaffold(
          appBar: AppBar(
            title: const Text('内部错误'),
          ),
          body: const Center(
            child: Text("请勿重复打开软件"),
          ),
        );
      },
    ));
    return;
  }
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.blue),
        useMaterial3: true,
      ),
      darkTheme: ThemeData(
        colorScheme: ColorScheme.fromSeed(
          seedColor: Colors.blue,
          brightness: Brightness.dark,
        ),
        useMaterial3: true,
      ),
      home: const LoginPage(),
    );
  }
}
