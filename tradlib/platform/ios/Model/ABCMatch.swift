//
//  ABCMatch.swift
//  Trad App
//
//  Created by damien murtagh on 1/8/23.
//

import Foundation

struct ABCMatch: Codable, CustomStringConvertible, Identifiable
{
    var id: Int
    var ed: Float // Edit Distance
    var line: String
    var corpusEntry: CorpusEntry

    var tags: [String]?
    
    var description: String {
        let tags = self.tags != nil ? self.tags! : []
        return "\(corpusEntry)" +
        "Edit Distance: \(ed)\n" +
        "Line: \(line)\n" +
        "Tags: \(tags)";
    }
}
