//
//  AudioCapture.swift
//  Trad App
//
//  Created by damien murtagh on 12/29/22.
//

import Foundation
import AVFoundation

enum AudioCaptureError: Error {
    case failedToOpenFile(_ filename: String)
    case failedToReadIntoBuffer(_ filename: String)
}

func loadWavFile(_ filename: String) throws -> AVAudioPCMBuffer?
{
    try loadFile(filename, withExtension: "wav")
}

func loadFile(_ filename: String, withExtension fileExtension: String) throws -> AVAudioPCMBuffer?
{
    print ("Loading: \(filename).\(fileExtension)")
    let fileURLOptional = Bundle.main.url(forResource: filename, withExtension: fileExtension)
    
    guard let unwrappedURL = fileURLOptional else
    {
        throw AudioCaptureError.failedToOpenFile(filename)
    }
    
    guard let audioFile = try? AVAudioFile(forReading: unwrappedURL, commonFormat: AVAudioCommonFormat.pcmFormatFloat32, interleaved: false) else
    //guard let audioFile = try? AVAudioFile(forReading: unwrappedURL, commonFormat: AVAudioCommonFormat.pcmFormatInt16, interleaved: true) else
    // Default (format not specified
    //guard let audioFile = try? AVAudioFile(forReading: unwrappedURL) else
    {
        throw AudioCaptureError.failedToOpenFile(filename)
    }
    
    print("Loading file of length: \(audioFile.length)")
    print("Format is \(audioFile.processingFormat.description)")
    
    let processingFormat = audioFile.processingFormat;
    let fileFormat = audioFile.fileFormat;
    let debugDescription = audioFile.debugDescription;
    let length = audioFile.length;
    
    guard let buffer = AVAudioPCMBuffer(pcmFormat: audioFile.processingFormat, frameCapacity: AVAudioFrameCount(audioFile.length)) else
    {
        throw AudioCaptureError.failedToReadIntoBuffer(filename)
    }
    
    do
    {
        try audioFile.read(into: buffer, frameCount: UInt32(length))
    }
    catch
    {
        throw AudioCaptureError.failedToReadIntoBuffer(filename)
    }
    
    let floatArray = PCMBufferToFloatArray(buffer);
    
    return buffer
}

 func PCMBufferToFloatArray(_ pcmBuffer: AVAudioPCMBuffer) -> [Float]?
 {
     if let int16ChannelData = pcmBuffer.int16ChannelData
     {
         let frameLength = Int(pcmBuffer.frameLength)
         let stride = pcmBuffer.stride
         var result: [Float] = Array(repeating: 0, count: frameLength);
         var temp: [Int16] = Array(repeating: 0, count: frameLength);
         
         for sampleIndex in 0..<frameLength
         {
             let int16Value = int16ChannelData[0][sampleIndex * stride];
             temp[sampleIndex] = int16Value;
             result[sampleIndex] = Float(int16Value);
         }
         return result;
     }
     
     if let int32ChannelData = pcmBuffer.int32ChannelData
     {
         let frameLength = Int(pcmBuffer.frameLength)
         let stride = pcmBuffer.stride
         var result: [Float] = Array(repeating: 0, count: frameLength);
         var temp: [Int32] = Array(repeating: 0, count: frameLength);
         
         for sampleIndex in 0..<frameLength
         {
             let intValue = int32ChannelData[0][sampleIndex * stride];
             temp[sampleIndex] = intValue;
             result[sampleIndex] = Float(intValue);
         }
         return result;
     }
     
     
     if let floatChannelData = pcmBuffer.floatChannelData
     {
         let frameLength = Int(pcmBuffer.frameLength)
         let stride = pcmBuffer.stride
         var result: [Float] = Array(repeating: 0.0, count: frameLength);

         for sampleIndex in 0..<frameLength
         {
             result[sampleIndex] = floatChannelData[0][sampleIndex * stride]
         }
         
         //print (result)
         return result
     }
     else
     {
         print("format not in Float")
         return nil
     }
 }

