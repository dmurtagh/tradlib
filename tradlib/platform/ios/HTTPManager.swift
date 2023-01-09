//
//  HTTPManager.swift
//  Trad App
//
//  Created by damien murtagh on 1/8/23.
//

import Foundation

class HTTPManager
{
    let domainUrlString = "http://localhost:8080/"
    
    func searchForCorpusEntry(searchTerm: String,
        completionHandler: @escaping ([ABCMatch]?) -> Void)
    {
        let url = URL(string: domainUrlString + "api/corpus-entry/search/" + searchTerm)!
        
        let task = URLSession.shared.dataTask(with: url, completionHandler: {
            (data, response, error) in
            if let error = error
            {
                print("Error calling api/corpus-entry/search/\(searchTerm): \(error)")
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
                    completionHandler(abcMatches)
                }
                catch {
                    print(error)
                    completionHandler(nil)
                }
            }
        })
        task.resume()
    }
}
