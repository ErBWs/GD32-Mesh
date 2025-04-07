import 'package:dio/dio.dart';
import 'package:flutter/foundation.dart';
import 'package:dio_cache_interceptor/dio_cache_interceptor.dart';

class Iot {
  final Dio dio = Dio(BaseOptions(
    connectTimeout: const Duration(milliseconds: 12000),
    receiveTimeout: const Duration(milliseconds: 12000),
  ));
  final CacheOptions _cacheOptions = CacheOptions(
    store: MemCacheStore(),
    policy: CachePolicy.forceCache,
    maxStale: const Duration(days: 1),
    priority: CachePriority.high,
  );

  Future<String> getAuthToken() async {
    dio.interceptors.add(DioCacheInterceptor(options: _cacheOptions));
    
    final response = await dio.post(
      "https://iam.cn-east-3.myhuaweicloud.com/v3/auth/tokens",
      options: Options(
        extra: _cacheOptions.copyWith(
          maxStale: const Duration(days: 1),
        ).toExtra(),
      ),
      data: {
        "auth": {
          "identity": {
            "methods": ["password"],
            "password": {
              "user": {
                "name": "user1",
                "password": "",
                "domain": {"name": "erbw_s"}
              }
            }
          },
          "scope": {
            "project": {"id": "be623ce1bf6c4ce9b9609768b1d4e68e"}
          }
        }
      },
    );

    return response.headers.value("X-Subject-Token") ?? "";
  }

  Future<Response> getShadowData(
      String projectID, String deviceID) async {
    String shadowUrl =
        "https://0c5e592236.st1.iotda-app.cn-east-3.myhuaweicloud.com/v5/iot/{project_id}/devices/{device_id}/shadow";
    shadowUrl = shadowUrl.replaceFirst("{project_id}", projectID);
    shadowUrl = shadowUrl.replaceFirst("{device_id}", deviceID);

    final token = await getAuthToken();
    debugPrint(token);

    Options options = Options(
      responseType: ResponseType.json,
      headers: {
        'X-Auth-Token': token,
      },
    );

    try {
      final response = await dio.get(shadowUrl, options: options);
      debugPrint(response.data.toString());
      return response;
    } catch (e) {
      debugPrint('请求失败: $e');
      rethrow;
    }
  }
}

Iot iot = Iot();