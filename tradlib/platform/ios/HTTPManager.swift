//
//  HTTPManager.swift
//  Trad App
//
//  Created by damien murtagh on 1/8/23.
//

import Foundation

class HTTPManager
{
    private var hostname = "http://tradapp-development-env.eba-gxfsrk6z.eu-west-1.elasticbeanstalk.com"
    private let awsHostname = "http://tradapp-development-env.eba-gxfsrk6z.eu-west-1.elasticbeanstalk.com"
    private let localHostname = "http://localhost:8080"
    private let localIP = "http://172.20.10.5:8080"
    
    private let serverEnvironmentKey = "serverEnvironment"
    
    init() {
        let env = UserDefaults.standard.string(forKey: serverEnvironmentKey) ?? "AWS"
        if env == "AWS" {
            hostname = awsHostname
        }
        else if env == "localhost" {
            hostname = localHostname
        }
        else if (env == "localIP")
        {
            hostname = localIP;
        }
        else {
            assert(false)
            print("invalid server environemnt")
        }
    }
    
    func getHostname() -> String {
        return hostname
    }
    
    func setAWSServer() {
        hostname = awsHostname
        UserDefaults.standard.set("AWS", forKey: serverEnvironmentKey)
    }
    
    func setLocalServer() {
        hostname = localHostname
        UserDefaults.standard.set("localhost", forKey: serverEnvironmentKey)
    }
    
    func setLocalIPServer() {
        hostname = localIP
        UserDefaults.standard.set("localIP", forKey: serverEnvironmentKey)
    }
    
    func searchForCorpusEntry(searchTerm: String,
        completionHandler: @escaping (Result<[ABCMatch]?, SearchError>) -> Void)
    {
        let url = URL(string: hostname + "/api/corpus-entry/search/" + searchTerm)!
        
        // Create a URLSessionConfiguration object with a custom timeout
        let configuration = URLSessionConfiguration.default
        configuration.timeoutIntervalForRequest = 10.0 
        
        let shortTimeoutSession = URLSession(configuration: configuration)
        
        let task = shortTimeoutSession.dataTask(with: url, completionHandler: {
            (data, response, error) in
            if let error = error
            {
                print("Error calling api/corpus-entry/search/\(searchTerm): \(error)")
                DispatchQueue.main.async {
                    completionHandler(.failure(.connectionTimeout))
                }
                return
            }
            
            guard let httpResponse = response as? HTTPURLResponse,
                  (200...299).contains(httpResponse.statusCode) else
            {
                print("Error with the response, unexpected status code: \(String(describing: response))")
                return
            }
            
            if let data = data, let string = String(data: data, encoding: .utf8){
                print(string)
            }
            
            if let data = data{
                do {
                    let abcMatches = try JSONDecoder().decode([ABCMatch].self, from: data)
                    // Make sure to respond on the main thread
                    DispatchQueue.main.async {
                        completionHandler(.success(abcMatches))
                    }
                }
                catch {
                    print(error)
                    // Make sure to respond on the main thread
                    DispatchQueue.main.async {
                        completionHandler(.failure(.dataParsingError))
                    }
                }
            }
        })
        task.resume()
    }
}
